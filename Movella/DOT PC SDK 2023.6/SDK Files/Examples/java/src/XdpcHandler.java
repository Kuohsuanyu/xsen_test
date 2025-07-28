
//  Copyright (c) 2003-2023 Movella Technologies B.V. or subsidiaries worldwide.
//  All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met:
//  
//  1.	Redistributions of source code must retain the above copyright notice,
//  	this list of conditions and the following disclaimer.
//  
//  2.	Redistributions in binary form must reproduce the above copyright notice,
//  	this list of conditions and the following disclaimer in the documentation
//  	and/or other materials provided with the distribution.
//  
//  3.	Neither the names of the copyright holders nor the names of their contributors
//  	may be used to endorse or promote products derived from this software without
//  	specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
//  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR
//  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  

import com.movella.movelladot_pc_sdk.*;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.LinkedList;
import java.util.Queue;
import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicLong;

import java.util.Scanner;
import java.util.Arrays;
import java.util.HashMap;

class XdpcHandler extends XsDotCallback
{
  private XsDotConnectionManager m_manager;
  
  private boolean m_errorReceived = false;
  private boolean m_updateDone = false;
  private boolean m_recordingStopped = false;
  private boolean m_exportDone = false;
  private boolean m_closing = false;
  private int m_progressCurrent = 0;
  private int m_progressTotal = 0;
  private int m_packetsReceived = 0;

  private XsPortInfoArray m_detectedDots = new XsPortInfoArray();
  private ArrayList<XsDotDevice> m_connectedDots = new ArrayList<>();
  private ArrayList<XsDotUsbDevice> m_connectedUsbDots = new ArrayList<>();

  private int m_maxNumberOfPacketsInBuffer = 5;
  private HashMap<String, AtomicLong> m_numberOfPacketsInBuffer = new HashMap<>();
  private HashMap<String, Queue<XsDataPacket>> m_packetBuffer = new HashMap<>();
  private HashMap<String, Integer> m_progressBuffer = new HashMap<>();

  /**
   * Initialize the PC SDK
   * <p>- Prints the used PC SDK version to show we connected to XDPC
   * <p>- Constructs the connection manager used for discovering and connecting to DOTs
   * <p>- Connects this class as callback handler to the XDPC
   * @return false if there was a problem creating a connection manager.
   */
  public Boolean initialize()
  {
    // Print SDK version
    XsVersion v = new XsVersion();
    movelladot_pc_sdk.xsdotsdkDllVersion(v);
    System.out.println(String.format("Using Movella DOT SDK version: %s", v.toXsString()));

    System.out.println("Creating Movella DOT Connection Manager object...");
    m_manager = XsDotConnectionManager.construct();

    if (m_manager == null)
    {
      System.out.println("Manager could not be constructed, exiting.");
      return false;
    }

    // Attach callback handler (this) to connection manager
    m_manager.addXsDotCallbackHandler(this);
    return true;
  }

  /**
   * Close connections to any Movella DOT devices and destructs the connection manager created in initialize
   */
  public void cleanup()
  {
    System.out.println("Closing ports...");
    m_closing = true;
    m_manager.close();

    System.out.println("Destructing XsDotConnectionManager object...");
    m_manager.destruct();

    System.out.println("Successful exit.");
  }

  /**
   * Scan if any Movella DOT devices can be detected via Bluetooth
   * <p>
   * Enables device detection in the connection manager and uses the
   * onAdvertisementFound callback to detect active Movella DOT devices
   * @throws Exception
   */
  public void scanForDots() throws Exception
  {
    // Start a scan and wait until we have found one or more Movella DOT Devices
    System.out.println("Scanning for devices...");
    m_manager.enableDeviceDetection();

    System.out.println("Press [ENTER] key or wait 20 seconds to stop scanning...");
    boolean waitForConnections = true;
    long connectedDotCount = 0;
    long startTime = XsTimeStamp.nowMs();
    Scanner keyboard = new Scanner(System.in);
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));    
    while(waitForConnections && !errorReceived() && XsTimeStamp.nowMs() - startTime <= 20000)
    {
      Thread.sleep(100);

      long nextCount = detectedDots().size();
      if (nextCount != connectedDotCount)
      {
        System.out.println(String.format("Number of detected DOTs: %d. Press [ENTER] key to start.", nextCount));
        connectedDotCount = nextCount;
      }
      if (br.ready())
        waitForConnections = false;
    }
    keyboard.close();

    m_manager.disableDeviceDetection();
    System.out.println("Stopped scanning for Devices.");
  }

  /**
   * Connects to Movella DOTs found via either USB or Bluetooth connection
   * <p>
   * Uses the isBluetooth function of the XsPortInfo to determine if the device was detected
   * via Bluetooth or via USB. Then connects to the device accordingly
   * When using Bluetooth, a retry has been built in, since wireless connection sometimes just fails the 1st time
   * Connected devices can be retrieved using either connectedDots() or connectedUsbDots()
   * <p>
   * USB and Bluetooth devices should not be mixed in the same session!
   */
  public void connectDots()
  {
    for (int i = 0; i < detectedDots().size(); ++i)
    {
      XsPortInfo portInfo = detectedDots().at(i);
      if (portInfo.isBluetooth())
      {      
        System.out.println("Opening DOT with address: " + portInfo.bluetoothAddress());
        if(!m_manager.openPort(portInfo))
        {
          System.out.println(String.format("Connection to Device %s failed, retrying...", portInfo.bluetoothAddress()));
          if(!m_manager.openPort(portInfo))
          {
            System.out.println(String.format("Could not open DOT. Reason: %s", m_manager.lastResultText().toString()));
            continue;
          }
        }

        XsDotDevice device = m_manager.device(portInfo.deviceId());
        if (device == null)
          continue;
        m_connectedDots.add(device);
        System.out.println(String.format("Found a device with tag: %s @ address: %s", device.deviceTagName(), device.bluetoothAddress()));
      }
      else
      {
        System.out.println(String.format("Opening DOT with ID: %s @ port: %s, baudrate: %s",
                            portInfo.deviceId().toXsString(),
                            portInfo.portName(),
                            portInfo.baudrate()));

        if(!m_manager.openPort(portInfo))
        {
          System.out.println(String.format("Could not open DOT. Reason: %s", m_manager.lastResultText().toString()));
          continue;
        }

        XsDotUsbDevice device = m_manager.usbDevice(portInfo.deviceId());
        if (device == null)
          continue;
        m_connectedUsbDots.add(device);
        System.out.println(String.format("Device: %s, with ID: %s opened.", device.productCode(), device.deviceId().toXsString()));
      }
    }
  }

  /**
   * Scans for USB connected Movella DOT devices for data export
   */
  public void detectUsbDevices()
  {
    System.out.println("Scanning for USB devices...");
    m_detectedDots = m_manager.detectUsbDevices();
  }

  /**
   * @return A pointer to the XsDotConnectionManager
   */
  public XsDotConnectionManager manager()
  {
    return m_manager;
  }

  /**
   * @return An XsPortInfoArray containing information on detected Movella DOT devices
   */
  public XsPortInfoArray detectedDots()
  {
    synchronized (this)
    {
      return m_detectedDots;
    }
  }

  /**
   * @return A list containing an XsDotDevice pointer for each Movella DOT device connected via Bluetooth
   */
  public ArrayList<XsDotDevice> connectedDots()
  {
    return m_connectedDots;
  }

  /**
   * @return A list containing an XsDotUsbDevice pointer for each Movella DOT device connected via USB
   */
  public ArrayList<XsDotUsbDevice> connectedUsbDots()
  {
    return m_connectedUsbDots;
  }

  /**
   * @return True if an error was received through the onError callback
   */
  public boolean errorReceived()
  {
    return m_errorReceived;
  }

  /**
   * @return True if the export has finished
   */
  public boolean exportDone()
  {
    return m_exportDone;
  }

  /**
   * @return Whether update done was received through the onDeviceUpdateDone callback
   */
  public boolean updateDone()
  {
    return m_updateDone;
  }

  /**
   * Resets the update done member variable to be ready for a next device update
   */
  public void resetUpdateDone()
  {
    m_updateDone = false;
  }

  /**
   * @return True if the device indicated the recording has stopped
   */
  public boolean recordingStopped()
  {
    return m_recordingStopped;
  }

  /**
   * Resets the recording stopped member variable to be ready for a next recording
   */
  public void resetRecordingStopped()
  {
    m_recordingStopped = false;
  }

  /**
   * @return True if a data packet is available for each of the connected Movella DOT devices
   */
  public boolean packetsAvailable()
  {
    for (long i = 0; i < m_detectedDots.size(); i++) {
      if (!packetAvailable(m_detectedDots.at(i).bluetoothAddress().toString()))
        return false;
    }
    return true;
  }

  /**
   * @param bluetoothAddress The bluetooth address of the Movella DOT to check for a ready data packet
   * @return True if a data packet is available for the Movella DOT with the provided bluetoothAddress
   */
  public boolean packetAvailable(String bluetoothAddress)
  {
    synchronized (this)
    {
      if (!m_numberOfPacketsInBuffer.containsKey(bluetoothAddress))
        return false;
      return m_numberOfPacketsInBuffer.get(bluetoothAddress).get() > 0;
    }
  }

  /**
   * @return The number of packets received during data export
   */
  public int packetsReceived()
  {
    return m_packetsReceived;
  }

  /**
   * @param bluetoothAddress The bluetooth address of the Movella DOT to get the next packet for
   * @return The next available data packet for the Movella DOT with the provided bluetoothAddress
   */
  public XsDataPacket getNextPacket(String bluetoothAddress)
  {
    if (!packetAvailable(bluetoothAddress))
      return new XsDataPacket();
    synchronized (this)
    {
      XsDataPacket oldestPacket = m_packetBuffer.get(bluetoothAddress).peek();
      m_packetBuffer.get(bluetoothAddress).remove();
      m_numberOfPacketsInBuffer.get(bluetoothAddress).decrementAndGet();
      return oldestPacket;
    }
  }

  /**
   * Initialize internal progress buffer for an Movella DOT device
   * @param bluetoothAddress The bluetooth address of the Movella DOT device
   */
  public void addDeviceToProgressBuffer(String bluetoothAddress)
  {
    m_progressBuffer.put(bluetoothAddress, 0);
  }

  /**
   * @param bluetoothAddress The bluetooth address of the Movella DOT device
   * @return The current progress indication of the Movella DOT with the provided bluetoothAddress
   */
  public int progress(String bluetoothAddress)
  {
    return m_progressBuffer.get(bluetoothAddress);
  }

  /**
   * Helper function for printing file export info to the command line.
   */
  private void outputDeviceProgress()
  {
    System.out.print("\rExporting... ");
    if (m_exportDone)
      System.out.println("done!");
    else if (m_progressTotal != 0xffff)
      System.out.print(String.format("%.1f%%", (double)(100 * m_progressCurrent) / m_progressTotal));
    else
      System.out.print(String.format("%d", m_progressCurrent));
  }

  /**
   * Called when an Movella DOT device advertisement was received. Updates m_detectedDots.
   * @param portInfo The XsPortInfo of the discovered information
   */
  protected void onAdvertisementFound(XsPortInfo portInfo)
  {
    synchronized (this)
    {
      if(UserSettings.whiteList.length == 0 || Arrays.asList(UserSettings.whiteList).contains(portInfo.bluetoothAddress().toString()))
        m_detectedDots.push_back(portInfo);
      else
        System.out.println("Ignoring " + portInfo.bluetoothAddress());
    }
  }

  /**
   * Called when a battery status update is available. Prints to screen.
   * @param device The device that initiated the callback
   * @param batteryLevel The battery level in percentage
   * @param chargingStatus The charging status of the battery. 0: Not charging, 1: charging
   */
  protected void onBatteryUpdated(XsDotDevice device, int batteryLevel, int chargingStatus)
  {
    System.out.println(String.format("%s BatteryLevel: %d Charging status: %d", device.portName(), batteryLevel, chargingStatus));
  }

  /**
   * Called when an internal error has occurred. Prints to screen.
   * @param result The XsResultValue related to this error
   * @param errorString The error string with information on the problem that occurred
   */
  protected void onError(XsResultValue result, XsString errorString)
  {
    System.out.println(result.toString());
    System.out.println(String.format("Error received: %s", errorString));
    m_errorReceived = true;
  }

  /**
   * Called when new data has been received from a device
   * <p>
   * Adds the new packet to the device's packet buffer
   * Monitors buffer size, removes oldest packets if the size gets too big
   * @param device The device that initiated the callback
   * @param packet The data packet that has been received (and processed)
   */
  protected void onLiveDataAvailable(XsDotDevice device, XsDataPacket packet)
  {
    synchronized (this)
    {
      String bluetoothAddress = device.bluetoothAddress().toString();
      if (m_numberOfPacketsInBuffer.containsKey(bluetoothAddress))
        while (m_numberOfPacketsInBuffer.get(bluetoothAddress).get() >= m_maxNumberOfPacketsInBuffer)
          getNextPacket(bluetoothAddress);

      m_packetBuffer.putIfAbsent(bluetoothAddress, new LinkedList<XsDataPacket>());
      m_packetBuffer.get(bluetoothAddress).add(new XsDataPacket(packet));

      m_numberOfPacketsInBuffer.putIfAbsent(bluetoothAddress, new AtomicLong(0));
      m_numberOfPacketsInBuffer.get(bluetoothAddress).incrementAndGet();
    }
  }

  /**
   * Called when a long-duration operation has made some progress or has completed.
   * @param device The device that initiated the callback
   * @param current The current progress
   * @param total The total work to be done. When \a current equals \a total, the task is completed
   * @param identifier An identifier for the task. This may for example be a filename for file read operations
   */
  protected void onProgressUpdated(XsDotDevice device, int current, int total, XsString identifier)
  {
    String address = device.bluetoothAddress().toString();
    if (!m_progressBuffer.containsKey(address))
      m_progressBuffer.put(address, current);
    if (current > m_progressBuffer.get(address))
    {
      m_progressBuffer.put(address, current);
      if (identifier != null)
      System.out.print(String.format("\rUpdate: %d Total: %d Remark: %s", current, total, identifier.toString()));
    else
      System.out.print(String.format("\rUpdate: %d Total: %d", current, total));
    }    
  }

  /**
   * Called when the firmware update process has completed. Prints to screen.
   * @param portInfo The XsPortInfo of the updated device
   * @param result The XsDotFirmwareUpdateResult of the firmware update
   */
  protected void onDeviceUpdateDone(XsPortInfo portInfo, XsDotFirmwareUpdateResult result)
  {
    System.out.println(String.format("%s Firmware Update done. Result: %s", portInfo.bluetoothAddress().toString(), movelladot_pc_sdk.XsDotFirmwareUpdateResultToString(result)));
    m_updateDone = true;
  }

  /**
   * Called when a recording has stopped. Prints to screen.
   * @param device The device that initiated the callback
   */
  protected void onRecordingStopped(XsDotDevice device)
  {
    System.out.println("");
    System.out.println(String.format("Recording stopped: %s", device.deviceTagName()));
    m_recordingStopped = true;
  }

  /**
   * Called when the device state has changed.
   * <p>
   * Used for removing/disconnecting the device when it indicates a power down.
   * @param device The device that initiated the callback
   * @param newState The new device state
   * @param oldState The old device state
   */
  protected void onDeviceStateChanged(XsDotDevice device, XsDeviceState newState, XsDeviceState oldState)
  {
    if (newState == XsDeviceState.XDS_Destructing && !m_closing)
    {
      m_connectedDots.remove(device);
      System.out.println(String.format("%s Device powered down", device.deviceTagName()));
    }
  }

  /**
   * Called when the device's button has been clicked. Prints to screen.
   * @param device The device that initiated the callback
   * @param timestamp The timestamp at which the button was clicked
   */
  protected void onButtonClicked(XsDotDevice device, long timestamp)
  {
    System.out.println(String.format("%s Button clicked at %d", device.deviceTagName(), timestamp));
  }

  /**
   * Called when a long-duration operation has made some progress or has completed. Used for printing data export progress information.
   * @param device The device that initiated the callback
   * @param current The current progress
   * @param total The total work to be done. When \a current equals \a total, the task is completed
   * @param identifier An identifier for the task. This may for example be a filename for file read operations
   */
  protected void onProgressUpdated(XsDotUsbDevice device, int current, int total, XsString identifier)
  {
    m_progressCurrent = current;
    m_progressTotal = total;
    outputDeviceProgress();
  }

  /**
   * Called when new data has been received from a device that is exporting a recording via USB.
   * <p>
   * The callback rate will be as fast as the data comes in and not necessarily reflect real-time.
   * For timing information, please refer to the SampletimeFine which is available when the Timestamp field is exported.
   * @param device The device that initiated the callback
   * @param packet The data packet that has been received
   */
  protected void onRecordedDataAvailable(XsDotUsbDevice device, XsDataPacket packet)
  {
    m_packetsReceived++;
  }

  /**
   * Called when a device that is exporting a recording is finished with exporting via USB.
   * <p>
   * This callback will occur in any sitation that stops the export of the recording,
   * such as	the export being completed, the export being stopped by request or an internal failure.
   * @param device The device that initiated the callback
   */
  protected void onRecordedDataDone(XsDotUsbDevice device)
  {
    m_exportDone = true;
    outputDeviceProgress();
  }

    /**
   * Called when new data has been received from a device that is exporting a recording via BLE.
   * <p>
   * The callback rate will be as fast as the data comes in and not necessarily reflect real-time.
   * For timing information, please refer to the SampletimeFine which is available when the Timestamp field is exported.
   * @param device The device that initiated the callback
   * @param packet The data packet that has been received
   */
  protected void onRecordedDataAvailable(XsDotDevice device, XsDataPacket packet)
  {
    m_packetsReceived++;
  }

  /**
   * Called when a device that is exporting a recording is finished with exporting via BLE.
   * <p>
   * This callback will occur in any sitation that stops the export of the recording,
   * such as	the export being completed, the export being stopped by request or an internal failure.
   * @param device The device that initiated the callback
   */
  protected void onRecordedDataDone(XsDotDevice device)
  {
    m_exportDone = true;
    outputDeviceProgress();
  }
}