
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

#ifndef XDPC_HANDLER
#define XDPC_HANDLER

#include <movelladot_pc_sdk.h>
#include <xscommon/xsens_mutex.h>
#include <list>

class XdpcHandler : public XsDotCallback
{
public:
	XdpcHandler(size_t maxBufferSize = 5);
	virtual ~XdpcHandler() noexcept;

	bool initialize();
	void scanForDots();
	void detectUsbDevices();
	void connectDots();
	void cleanup();

	XsDotConnectionManager* manager() const;

	XsPortInfoArray detectedDots() const;
	std::list<XsDotDevice*> connectedDots() const;
	std::list<XsDotUsbDevice*> connectedUsbDots() const;
	bool errorReceived() const;
	bool exportDone() const;
	bool updateDone() const;
	void resetUpdateDone();
	bool recordingStopped() const;
	void resetRecordingStopped();
	bool packetsAvailable() const;
	bool packetAvailable(const XsString& bluetoothAddress) const;
	XsDataPacket getNextPacket(const XsString& bluetoothAddress);
	int packetsReceived() const;
	void addDeviceToProgressBuffer(XsString bluetoothAddress);
	int progress(XsString bluetoothAddress);

protected:
	void onAdvertisementFound(const XsPortInfo* portInfo) override;
	void onBatteryUpdated(XsDotDevice* device, int batteryLevel, int chargingStatus) override;
	void onLiveDataAvailable(XsDotDevice* device, const XsDataPacket* packet) override;
	void onProgressUpdated(XsDotDevice* device, int current, int total, const XsString* identifier) override;
	void onDeviceUpdateDone(const XsPortInfo* portInfo, XsDotFirmwareUpdateResult result) override;
	void onError(XsResultValue result, const XsString* error) override;
	void onRecordingStopped(XsDotDevice* device) override;
	void onDeviceStateChanged(XsDotDevice* device, XsDeviceState newState, XsDeviceState oldState) override;
	void onButtonClicked(XsDotDevice* device, uint32_t timestamp) override;
	void onProgressUpdated(XsDotUsbDevice* device, int current, int total, const XsString* identifier) override;
	void onRecordedDataAvailable(XsDotUsbDevice* device, const XsDataPacket* packet) override;
	void onRecordedDataAvailable(XsDotDevice* device, const XsDataPacket* packet) override;
	void onRecordedDataDone(XsDotUsbDevice* device) override;
	void onRecordedDataDone(XsDotDevice* device) override;

private:
	void outputDeviceProgress() const;

	XsDotConnectionManager* m_manager = nullptr;

	mutable xsens::Mutex m_mutex;
	bool m_errorReceived = false;
	bool m_updateDone = false;
	bool m_recordingStopped = false;
	bool m_exportDone = false;
	bool m_closing = false;
	int m_progressCurrent = 0;
	int m_progressTotal = 0;
	int m_packetsReceived = 0;
	XsPortInfoArray m_detectedDots;
	std::list<XsDotDevice*> m_connectedDots;
	std::list<XsDotUsbDevice*> m_connectedUsbDots;

	size_t m_maxNumberOfPacketsInBuffer;
	std::map<XsString, size_t> m_numberOfPacketsInBuffer;
	std::map<XsString, std::list<XsDataPacket>> m_packetBuffer;
	std::map<XsString, int> m_progressBuffer;
};

#endif

