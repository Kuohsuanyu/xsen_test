# -*- coding: utf-8 -*-
import os
import csv
import time
import argparse
from datetime import datetime

import movelladot_pc_sdk
from xdpchandler import XdpcHandler


def sanitize_filename(s: str) -> str:
    return str(s).replace(":", "_").replace("/", "_").replace("\\", "_")


def pick_payload_mode(device):
    """
    從 SDK 裡「真的存在」的 XsPayloadMode_* 常數中，挑一個最可能包含
    四元數/歐拉角/九軸的 mode 來呼叫 startMeasurement(mode)。
    如果全部失敗，再嘗試不帶參數版本。
    """
    if not hasattr(device, "startMeasurement"):
        return False, None

    names = [n for n in dir(movelladot_pc_sdk) if n.startswith("XsPayloadMode")]
    # 依常見需求排序（若 SDK 沒有會跳過）
    prefer = [
        "XsPayloadMode_ExtendedQuaternion",
        "XsPayloadMode_ExtendedEuler",
        "XsPayloadMode_Full",
        "XsPayloadMode_Calibrated",
        "XsPayloadMode_AccGyrMag",
    ]
    ordered = [n for n in prefer if n in names] + [n for n in names if n not in prefer]

    for name in ordered:
        try:
            mode = getattr(movelladot_pc_sdk, name)
            device.startMeasurement(mode)
            print(f"[INFO] startMeasurement OK with mode: {name}")
            return True, name
        except Exception as e:
            print(f"[WARN] startMeasurement({name}) failed: {e}")

    # 試試無參數版本（有些舊版可行）
    try:
        device.startMeasurement()
        print("[INFO] startMeasurement() without mode OK")
        return True, None
    except Exception as e:
        print(f"[WARN] startMeasurement() (no arg) failed: {e}")

    return False, None


def extract_fields(packet):
    """
    從 XsDataPacket 盡可能取出以下資料：
    - sampleTimeFine
    - status
    - quaternion (qw, qx, qy, qz)
    - euler (roll, pitch, yaw)
    - acc (ax, ay, az)
    - gyr (gx, gy, gz)
    - mag (mx, my, mz)

    若封包沒有，則回傳空字串。
    """
    stf = ""
    status = ""
    qw = qx = qy = qz = ""
    roll = pitch = yaw = ""
    ax = ay = az = ""
    gx = gy = gz = ""
    mx = my = mz = ""

    # sampleTimeFine / status
    try:
        if hasattr(packet, "sampleTimeFine"):
            stf = packet.sampleTimeFine()
    except Exception:
        pass

    try:
        if hasattr(packet, "status"):
            status = packet.status()
    except Exception:
        pass

    # Quaternion or Euler
    try:
        # 有些 SDK 用 containsOrientation() 統一判斷
        if hasattr(packet, "containsOrientation") and packet.containsOrientation():
            # 有 quaternion 的先拿 quaternion
            if hasattr(packet, "orientationQuaternion"):
                q = packet.orientationQuaternion()
                qw, qx, qy, qz = q.w(), q.x(), q.y(), q.z()
            # 也許同時有/或只有 euler
            if hasattr(packet, "orientationEuler"):
                e = packet.orientationEuler()
                roll, pitch, yaw = e.x(), e.y(), e.z()
        else:
            # 沒有 containsOrientation() 的話，各別嘗試
            if hasattr(packet, "orientationQuaternion"):
                q = packet.orientationQuaternion()
                qw, qx, qy, qz = q.w(), q.x(), q.y(), q.z()
            if hasattr(packet, "orientationEuler"):
                e = packet.orientationEuler()
                roll, pitch, yaw = e.x(), e.y(), e.z()
    except Exception:
        pass

    # Acc
    try:
        if hasattr(packet, "calibratedAcceleration"):
            a = packet.calibratedAcceleration()
            ax, ay, az = a[0], a[1], a[2]
    except Exception:
        pass

    # Gyr
    try:
        if hasattr(packet, "calibratedGyroscopeData"):
            g = packet.calibratedGyroscopeData()
            gx, gy, gz = g[0], g[1], g[2]
    except Exception:
        pass

    # Mag
    try:
        if hasattr(packet, "calibratedMagneticField"):
            m = packet.calibratedMagneticField()
            mx, my, mz = m[0], m[1], m[2]
    except Exception:
        pass

    return {
        "sampleTimeFine": stf,
        "status": status,
        "qw": qw, "qx": qx, "qy": qy, "qz": qz,
        "roll": roll, "pitch": pitch, "yaw": yaw,
        "ax": ax, "ay": ay, "az": az,
        "gx": gx, "gy": gy, "gz": gz,
        "mx": mx, "my": my, "mz": mz
    }


def main():
    parser = argparse.ArgumentParser(
        description="Movella DOT：即時錄製 (Quaternion / Euler / Acc / Gyr / Mag) 到 CSV，適用模型動作辨識資料收集")
    parser.add_argument("--outdir", type=str, default="./dot_records", help="輸出資料夾")
    parser.add_argument("--action", type=str, default="session1", help="錄製動作/場次名稱，會當成子資料夾名")
    parser.add_argument("--duration", type=float, default=0.0,
                        help="錄製秒數（>0 表示自動停止；<=0 則一直錄到 Ctrl+C）")
    parser.add_argument("--rate", type=int, default=60, help="嘗試設定輸出頻率（Hz），可能會因 SDK/裝置模式不同而失敗")
    parser.add_argument("--filter", type=str, default="General",
                        help="嘗試設定的 onboard filter profile 名稱（例如 General）")
    parser.add_argument("--bt", action="store_true", help="使用 Bluetooth (預設)")
    parser.add_argument("--usb", action="store_true", help="使用 USB 匯出（非即時）- 這支程式只做即時，USB 匯出請用官方範例一")

    args = parser.parse_args()

    if args.usb:
        print("這支程式是『即時』錄製，不支援 USB 匯出。USB 匯出請用官方 export 範例（你貼的第一段）。")
        return

    os.makedirs(os.path.join(args.outdir, args.action), exist_ok=True)

    handler = XdpcHandler()
    if not handler.initialize():
        print("初始化失敗，離開。")
        handler.cleanup()
        return

    # --- 掃描 & 連線（BT） ---
    handler.scanForDots()
    detected = handler.detectedDots()
    if len(detected) == 0:
        print("找不到裝置。")
        handler.cleanup()
        return

    handler.connectDots()
    devices = handler.connectedDots()
    if len(devices) == 0:
        print("連線失敗。")
        handler.cleanup()
        return

    print(f"✅ 已連線 {len(devices)} 顆 DOT：")
    for d in devices:
        print(f"  · {d.deviceTagName()} | {d.bluetoothAddress()}")

    # 嘗試設 filter profile / output rate（若 SDK 不支援會跳錯但不中斷）
    for d in devices:
        if hasattr(d, "setOnboardFilterProfile"):
            try:
                if d.setOnboardFilterProfile(args.filter):
                    print(f"[{d.bluetoothAddress()}] setOnboardFilterProfile('{args.filter}') OK")
                else:
                    print(f"[{d.bluetoothAddress()}] setOnboardFilterProfile('{args.filter}') failed: {d.lastResultText()}")
            except Exception as e:
                print(f"[{d.bluetoothAddress()}] setOnboardFilterProfile error: {e}")

        if hasattr(d, "setOutputRate") and args.rate > 0:
            try:
                if d.setOutputRate(args.rate):
                    print(f"[{d.bluetoothAddress()}] setOutputRate({args.rate}) OK")
                else:
                    print(f"[{d.bluetoothAddress()}] setOutputRate({args.rate}) failed: {d.lastResultText()}")
            except Exception as e:
                print(f"[{d.bluetoothAddress()}] setOutputRate error: {e}")

    # --- 建 CSV writer ---
    writers = {}
    files = {}
    action_dir = os.path.join(args.outdir, args.action)
    for d in devices:
        mac_safe = sanitize_filename(d.bluetoothAddress())
        filename = os.path.join(action_dir, f"{mac_safe}_live.csv")
        f = open(filename, "w", newline='', encoding="utf-8")
        w = csv.writer(f)
        w.writerow([
            "TimeISO", "MAC", "Name",
            "sampleTimeFine", "status",
            "qw", "qx", "qy", "qz",
            "roll", "pitch", "yaw",
            "ax", "ay", "az",
            "gx", "gy", "gz",
            "mx", "my", "mz"
        ])
        writers[d] = w
        files[d] = f

    # --- 啟動量測 ---
    for d in devices:
        ok, used_mode = pick_payload_mode(d)
        if not ok:
            print(f"[{d.bluetoothAddress()}] 無法 startMeasurement()，若 onLiveDataAvailable 有進資料仍會寫檔。")
        else:
            print(f"[{d.bluetoothAddress()}] startMeasurement OK, mode = {used_mode}")

    # --- 寫 loop ---
    print("開始錄製... Ctrl+C 可中止。")
    start = time.time()
    try:
        while True:
            # auto stop
            if args.duration > 0 and (time.time() - start) > args.duration:
                print("到達設定秒數，自動停止。")
                break

            for d in devices:
                addr = d.bluetoothAddress()
                # 注意：xdpchandler 使用 device.portInfo().bluetoothAddress() 當 key
                # 但我們這裡用的是 device.bluetoothAddress() ；兩者應該一致
                while handler.packetAvailable(addr):
                    pkt = handler.getNextPacket(addr)
                    if pkt is None:
                        break
                    fields = extract_fields(pkt)

                    writers[d].writerow([
                        datetime.now().isoformat(),
                        d.bluetoothAddress(),
                        d.deviceTagName(),
                        fields["sampleTimeFine"],
                        fields["status"],
                        fields["qw"], fields["qx"], fields["qy"], fields["qz"],
                        fields["roll"], fields["pitch"], fields["yaw"],
                        fields["ax"], fields["ay"], fields["az"],
                        fields["gx"], fields["gy"], fields["gz"],
                        fields["mx"], fields["my"], fields["mz"]
                    ])

            time.sleep(0.001)  # 放輕鬆一點 CPU
    except KeyboardInterrupt:
        print("\n收到 Ctrl+C，中止錄製。")

    # --- 停止 & 關閉 ---
    print("停止 measurement / 關閉檔案 / 清理連線 ...")
    for d in devices:
        try:
            if hasattr(d, "stopMeasurement"):
                d.stopMeasurement()
        except Exception:
            pass

    for f in files.values():
        try:
            f.close()
        except Exception:
            pass

    handler.cleanup()
    print("✅ 完成！CSV 已寫入：", action_dir)


if __name__ == "__main__":
    main()
