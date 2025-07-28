
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

#include "xdpchandler.h"

#include "user_settings.h"
#include "conio.h"

#include <iostream>
#include <iomanip>

using namespace std;

/*! \brief Constructor */
XdpcHandler::XdpcHandler(size_t maxBufferSize)
	: m_maxNumberOfPacketsInBuffer(maxBufferSize)
{
}

/*! \brief Destructor */
XdpcHandler::~XdpcHandler() noexcept
{
}

/*! \brief Initialize the PC SDK
	\details
	- Prints the used PC SDK version to show we connected to XDPC
	- Constructs the connection manager used for discovering and connecting to DOTs
	- Connects this class as callback handler to the XDPC
	\returns false if there was a problem creating a connection manager.
*/
bool XdpcHandler::initialize()
{
	// Print SDK version
	XsVersion version;
	xsdotsdkDllVersion(&version);
	cout << "Using Movella DOT SDK version: " << version.toString().toStdString() << endl;

	// Create connection manager
	cout << "Creating Movella DOT Connection Manager object..." << endl;
	m_manager = XsDotConnectionManager::construct();
	if (m_manager == nullptr)
	{
		cout << "Manager could not be constructed, exiting." << endl;
		return false;
	}

	// Attach callback handler (this) to connection manager
	m_manager->addXsDotCallbackHandler(this);

	return true;
}

/*! \brief Close connections to any Movella DOT devices and destructs the connection manager created in initialize */
void XdpcHandler::cleanup()
{
	if (!m_manager)
		return;

	cout << "Closing ports..." << endl;
	m_closing = true;
	m_manager->close();

	cout << "Freeing XsDotConnectionManager object..." << endl;
	m_manager->destruct();
	m_manager = nullptr;

	cout << "Successful exit." << endl;
}

/*! \brief Scan if any Movella DOT devices can be detected via Bluetooth
	\details Enables device detection in the connection manager and uses the
	onAdvertisementFound callback to detect active Movella DOT devices
	Disables device detection when done
*/
void XdpcHandler::scanForDots()
{
	// Optionally, on Linux you can use another available bluetooth adapter using the name
	// Note that this adapter needs to be changed prior to enabling Device Detection, uncomment the lines below
	// cout << "Setting bluetooth adapter to use" << endl;
	// auto adapterList = manager->getAvailableBluetoothAdapters();
	// for (auto& adapter : adapterList)
	// 	cout << "Found Bluetooth adapter: " << adapter << endl;

	// bool setAdapter = manager->setPreferredBluetoothAdapter("hci1");
	// if (!setAdapter)
	// 	cout << "Could not set adapter. It does not exist or is not available." << endl;
	// else
	// 	cout << "Successfully set Preferred Adapter" << endl;

	// Start a scan and wait until we have found one or more Movella DOT Devices
	cout << "Scanning for devices..." << endl;
	m_manager->enableDeviceDetection();

	cout << "Press any key or wait 20 seconds to stop scanning..." << endl;
	bool waitForConnections = true;
	size_t connectedDOTCount = 0;
	int64_t startTime = XsTime::timeStampNow();
	do
	{
		XsTime::msleep(100);

		size_t nextCount = detectedDots().size();
		if (nextCount != connectedDOTCount)
		{
			cout << "Number of detected DOTs: " << nextCount << ". Press any key to start." << endl;
			connectedDOTCount = nextCount;
		}
		if (_kbhit())
			waitForConnections = false;
	} while (waitForConnections && !errorReceived() && (XsTime::timeStampNow() - startTime <= 20000));

	m_manager->disableDeviceDetection();
	cout << "Stopped scanning for devices." << endl;
}

/*! \brief Connects to Movella DOTs found via either USB or Bluetooth connection
	\details Uses the isBluetooth function of the XsPortInfo to determine if the device was detected
	via Bluetooth or via USB. Then connects to the device accordingly
	When using Bluetooth, a retry has been built in, since wireless connection sometimes just fails the 1st time
	Connected devices can be retrieved using either connectedDots() or connectedUsbDots()
	\note USB and Bluetooth devices should not be mixed in the same session!
*/
void XdpcHandler::connectDots()
{
	for (auto& portInfo : detectedDots())
	{
		if (portInfo.isBluetooth())
		{
			cout << "Opening DOT with address: " << portInfo.bluetoothAddress() << endl;
			if (!m_manager->openPort(portInfo))
			{
				cout << "Connection to Device " << portInfo.bluetoothAddress() << " failed, retrying..." << endl;
				cout << "Device " << portInfo.bluetoothAddress() << " retry connected: " << endl;
				if (!m_manager->openPort(portInfo))
				{
					cout << "Could not open DOT. Reason: " << m_manager->lastResultText() << endl;
					continue;
				}
			}
			XsDotDevice* device = m_manager->device(portInfo.deviceId());

			if (device == nullptr)
				continue;

			m_connectedDots.push_back(device);
			cout << "Found a device with tag: " << device->deviceTagName().toStdString() << " @ address: " << device->bluetoothAddress() << endl;
		}
		else
		{
			cout << "Opening DOT with ID: " << portInfo.deviceId().toString().toStdString() << " @ port: " << portInfo.portName().toStdString() << ", baudrate: " << portInfo.baudrate() << endl;
			if (!m_manager->openPort(portInfo))
			{
				cout << "Could not open DOT. Reason: " << m_manager->lastResultText() << endl;
				continue;
			}
			XsDotUsbDevice* device = m_manager->usbDevice(portInfo.deviceId());
			if (device == nullptr)
				continue;

			m_connectedUsbDots.push_back(device);
			cout << "Device: " << device->productCode().toStdString() << ", with ID: " << device->deviceId().toString() << " opened." << endl;
		}
	}
}

/*! \brief Scans for USB connected Movella DOT devices for data export */
void XdpcHandler::detectUsbDevices()
{
	cout << "Scanning for USB devices..." << endl;
	m_detectedDots = m_manager->detectUsbDevices();
}

/*! \returns A pointer to the XsDotConnectionManager */
XsDotConnectionManager* XdpcHandler::manager() const
{
	return m_manager;
}

/*! \returns An XsPortInfoArray containing information on detected Movella DOT devices */
XsPortInfoArray XdpcHandler::detectedDots() const
{
	xsens::Lock locky(&m_mutex);
	return m_detectedDots;
}

/*! \returns A list containing an XsDotDevice pointer for each Movella DOT device connected via Bluetooth */
list<XsDotDevice*> XdpcHandler::connectedDots() const
{
	return m_connectedDots;
}

/*! \returns A list containing an XsDotUsbDevice pointer for each Movella DOT device connected via USB */
list<XsDotUsbDevice*> XdpcHandler::connectedUsbDots() const
{
	return m_connectedUsbDots;
}

/*! \returns True if an error was received through the onError callback */
bool XdpcHandler::errorReceived() const
{
	return m_errorReceived;
}

/*! \returns True if the export has finished */
bool XdpcHandler::exportDone() const
{
	return m_exportDone;
}

/*! \returns Whether update done was received through the onDeviceUpdateDone callback */
bool XdpcHandler::updateDone() const
{
	return m_updateDone;
}

/*! \brief Resets the update done member variable to be ready for a next device update
*/
void XdpcHandler::resetUpdateDone()
{
	m_updateDone = false;
}

/*! \returns True if the device indicated the recording has stopped */
bool XdpcHandler::recordingStopped() const
{
	return m_recordingStopped;
}

/*! \brief Resets the recording stopped member variable to be ready for a next recording */
void XdpcHandler::resetRecordingStopped()
{
	m_recordingStopped = false;
}

/*! \returns True if a data packet is available for each of the connected Movella DOT devices */
bool XdpcHandler::packetsAvailable() const
{
	for (auto const& device : m_connectedDots)
		if (!packetAvailable(device->bluetoothAddress()))
			return false;
	return true;
}

/*! \returns True if a data packet is available for the Movella DOT with the provided bluetoothAddress
	\param bluetoothAddress The bluetooth address of the Movella DOT to check for a ready data packet
*/
bool XdpcHandler::packetAvailable(const XsString& bluetoothAddress) const
{
	xsens::Lock locky(&m_mutex);
	if (m_numberOfPacketsInBuffer.find(bluetoothAddress) == m_numberOfPacketsInBuffer.end())
		return false;
	return m_numberOfPacketsInBuffer.at(bluetoothAddress) > 0;
}

/*! \returns The number of packets received during data export */
int XdpcHandler::packetsReceived() const
{
	return m_packetsReceived;
}

/*! \returns The next available data packet for the Movella DOT with the provided bluetoothAddress
	\param bluetoothAddress The bluetooth address of the Movella DOT to get the next packet for
*/
XsDataPacket XdpcHandler::getNextPacket(const XsString& bluetoothAddress)
{
	if (!packetAvailable(bluetoothAddress))
		return XsDataPacket();
	xsens::Lock locky(&m_mutex);
	XsDataPacket oldestPacket(m_packetBuffer[bluetoothAddress].front());
	m_packetBuffer[bluetoothAddress].pop_front();
	--m_numberOfPacketsInBuffer[bluetoothAddress];

	return oldestPacket;
}

/*! \brief Initialize internal progress buffer for an Movella DOT device
	\param bluetoothAddress The bluetooth address of the Movella DOT device
*/
void XdpcHandler::addDeviceToProgressBuffer(XsString bluetoothAddress)
{
	m_progressBuffer.insert(pair<XsString, int>(bluetoothAddress, 0));
}

/*! \returns The current progress indication of the Movella DOT with the provided bluetoothAddress
	\param bluetoothAddress The bluetooth address of the Movella DOT device
*/
int XdpcHandler::progress(XsString bluetoothAddress)
{
	return m_progressBuffer[bluetoothAddress];
}

/*! \brief Helper function for printing file export info to the command line. */
void XdpcHandler::outputDeviceProgress() const
{
	cout << "\rExporting... ";
	if (m_exportDone)
		cout << "done!" << endl;
	else if (m_progressTotal != 0xffff)
		cout << fixed << setprecision(1) << (100.0 * m_progressCurrent) / m_progressTotal << '%' << flush;
	else
		cout << m_progressCurrent << flush;
}

/*! \brief Called when an Movella DOT device advertisement was received. Updates m_detectedDots.
	\param portInfo The XsPortInfo of the discovered information
 */
void XdpcHandler::onAdvertisementFound(const XsPortInfo* portInfo)
{
	xsens::Lock locky(&m_mutex);
	if (!UserSettings().m_whiteList.size() || UserSettings().m_whiteList.find(portInfo->bluetoothAddress()) != -1)
		m_detectedDots.push_back(*portInfo);
	else
		cout << "Ignoring " << portInfo->bluetoothAddress() << endl;
}

/*! \brief Called when a battery status update is available. Prints to screen.
	\param device The device that initiated the callback
	\param batteryLevel The battery level in percentage
	\param chargingStatus The charging status of the battery. 0: Not charging, 1: charging
*/
void XdpcHandler::onBatteryUpdated(XsDotDevice* device, int batteryLevel, int chargingStatus)
{
	cout << device->deviceTagName() << " BatteryLevel: " << batteryLevel << " Charging status: " << chargingStatus;
	cout << endl;
}

/*! \brief Called when an internal error has occurred. Prints to screen.
	\param result The XsResultValue related to this error
	\param errorString The error string with information on the problem that occurred
*/
void XdpcHandler::onError(XsResultValue result, const XsString* errorString)
{
	cout << XsResultValue_toString(result) << endl;
	cout << "Error received: " << *errorString << endl;
	m_errorReceived = true;
}

/*! \brief Called when new data has been received from a device
	\details Adds the new packet to the device's packet buffer
	Monitors buffer size, removes oldest packets if the size gets too big
	\param device The device that initiated the callback
	\param packet The data packet that has been received (and processed)
*/
void XdpcHandler::onLiveDataAvailable(XsDotDevice* device, const XsDataPacket* packet)
{
	xsens::Lock locky(&m_mutex);
	assert(packet != nullptr);
	while (m_numberOfPacketsInBuffer[device->bluetoothAddress()] >= m_maxNumberOfPacketsInBuffer)
		(void)getNextPacket(device->bluetoothAddress());

	m_packetBuffer[device->bluetoothAddress()].push_back(*packet);
	++m_numberOfPacketsInBuffer[device->bluetoothAddress()];
}

/*! \brief Called when a long-duration operation has made some progress or has completed.
	\param device The device that initiated the callback
	\param current The current progress
	\param total The total work to be done. When \a current equals \a total, the task is completed
	\param identifier An identifier for the task. This may for example be a filename for file read operations
*/
void XdpcHandler::onProgressUpdated(XsDotDevice* device, int current, int total, const XsString* identifier)
{
	XsString address = device->bluetoothAddress();
	if (current > m_progressBuffer[address])
	{
		m_progressBuffer[address] = current;
		cout << "\r";
		if (identifier)
			cout << "Update: " << current << " Total: " << total << " Remark: " << *identifier;
		else
			cout << "Update: " << current << " Total: " << total;
		cout << flush;
	}
}

/*! \brief Called when the firmware update process has completed. Prints to screen.
	\param portInfo The XsPortInfo of the updated device
	\param result The XsDotFirmwareUpdateResult of the firmware update
*/
void XdpcHandler::onDeviceUpdateDone(const XsPortInfo* portInfo, XsDotFirmwareUpdateResult result)
{
	cout << endl << portInfo->bluetoothAddress() << " Firmware Update done. Result: " << XsDotFirmwareUpdateResult_toString(result) << endl;
	m_updateDone = true;
}

/*! \brief Called when a recording has stopped. Prints to screen.
	\param device The device that initiated the callback
*/
void XdpcHandler::onRecordingStopped(XsDotDevice* device)
{
	cout << endl << device->deviceTagName() << " Recording stopped" << endl;
	m_recordingStopped = true;
}

/*! \brief Called when the device state has changed.
	\details Used for removing/disconnecting the device when it indicates a power down.
	\param newState The new device state
	\param oldState The old device state
*/
void XdpcHandler::onDeviceStateChanged(XsDotDevice* device, XsDeviceState newState, XsDeviceState oldState)
{
	(void)oldState;
	if (newState == XDS_Destructing && !m_closing)
	{
		cout << endl << device->deviceTagName() << " Device powered down" << endl;
		m_connectedDots.remove(device);
	}
}

/*! \brief Called when the device's button has been clicked. Prints to screen.
	\param device The device that initiated the callback.
	\param timestamp The timestamp at which the button was clicked
*/
void XdpcHandler::onButtonClicked(XsDotDevice* device, uint32_t timestamp)
{
	cout << endl << device->deviceTagName() << " Button clicked at " << std::dec << timestamp << "(" << std::hex << timestamp << ")" << endl;
}

/*! \brief Called when a long-duration operation has made some progress or has completed. Used for printing data export progress information.
	\param device The device that initiated the callback
	\param current The current progress
	\param total The total work to be done. When \a current equals \a total, the task is completed
	\param identifier An identifier for the task. This may for example be a filename for file read operations
*/
void XdpcHandler::onProgressUpdated(XsDotUsbDevice* device, int current, int total, const XsString* identifier)
{
	(void)device;
	(void)identifier;
	m_progressCurrent = current;
	m_progressTotal = total;
	outputDeviceProgress();
}

/*! \brief Called when new data has been received from a device that is exporting a recording via USB.
    \details The callback rate will be as fast as the data comes in and not necessarily reflect real-time. For
	timing information, please refer to the SampletimeFine which is available when the Timestamp field is exported.
	\param device The device that initiated the callback
	\param packet The data packet that has been received
*/
void XdpcHandler::onRecordedDataAvailable(XsDotUsbDevice* device, const XsDataPacket* packet)
{
	(void)device;
	(void)packet;
	m_packetsReceived++;
}


/*! \brief Called when a device that is exporting via USB is finished with exporting a recording.
    \details This callback will occur in any sitation that stops the export of the recording, such as
	the export being completed, the export being stopped by request or an internal failure.
	\param device The device that initiated the callback
*/
void XdpcHandler::onRecordedDataDone(XsDotUsbDevice* device)
{
	(void)device;
	m_exportDone = true;
	outputDeviceProgress();
}

/*! \brief Called when a device that is exporting via BLE a recording is finished with exporting.
	\details This callback will occur in any sitation that stops the export of the recording, such as
	the export being completed, the export being stopped by request or an internal failure.
	\param device The device that initiated the callback
*/
void XdpcHandler::onRecordedDataDone(XsDotDevice* device)
{
	(void)device;
	m_exportDone = true;
	outputDeviceProgress();
}

/*! \brief Called when new data has been received from a device that is exporting a recording via BLE.
	\details The callback rate will be as fast as the data comes in and not necessarily reflect real-time. For
	timing information, please refer to the SampletimeFine which is available when the Timestamp field is exported.
	\param device The device that initiated the callback
	\param packet The data packet that has been received
*/
void XdpcHandler::onRecordedDataAvailable(XsDotDevice* device, const XsDataPacket* packet)
{
	(void)device;
	(void)packet;
	m_packetsReceived++;
}