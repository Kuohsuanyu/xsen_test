
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

//--------------------------------------------------------------------------------
// Movella DOT SDK C++ example Data Export.
//--------------------------------------------------------------------------------
#include <iostream>
#include <iomanip>

#include "xdpchandler.h"

using namespace std;

//--------------------------------------------------------------------------------
int main(void)
{
	XdpcHandler xdpcHandler;

	if (!xdpcHandler.initialize())
		return -1;

	// Start a scan for USB connected Movella DOT Devices
	xdpcHandler.detectUsbDevices();

	if (xdpcHandler.detectedDots().empty())
	{
		cout << "No Movella DOT device(s) found. Aborting." << endl;
		return -1;
	}

	xdpcHandler.connectDots();

	if (xdpcHandler.connectedUsbDots().empty())
	{
		cout << "Could not connect to any Movella DOT device(s). Aborting." << endl;
		return -1;
	}

	XsIntArray exportData;
	exportData.push_back(RecordingData_Timestamp);
	exportData.push_back(RecordingData_Euler);
	exportData.push_back(RecordingData_Acceleration);
	exportData.push_back(RecordingData_AngularVelocity);
	exportData.push_back(RecordingData_MagneticField);
	exportData.push_back(RecordingData_Status);

	XsDotUsbDevice* device = xdpcHandler.connectedUsbDots().front();

	int recordingIndex = device->recordingCount();
	XsRecordingInfo recInfo = device->getRecordingInfo(recordingIndex);
	if (recInfo.empty())
	{
		cout << "Could not get recording info. Reason: " << device->lastResultText() << endl;
	}
	else
	{
		cout << "Recording [" << recordingIndex << "], Storage Size: " << recInfo.storageSize() << " bytes" << endl;
		cout << "Recording [" << recordingIndex << "], Recording Time: " << recInfo.totalRecordingTime() << " seconds" << endl;
	}

	XsString csvFilename = XsString() << "device_" << device->deviceId().toString() << '_' << recordingIndex << ".csv";
	cout << "Exporting recording " << recordingIndex << " to file " << csvFilename << endl;

	if (!device->selectExportData(exportData))
	{
		cout << "Could not select export data. Reason: " << device->lastResultText() << endl;
	}
	else if (!device->enableLogging(csvFilename))
	{
		cout << "Could not open logfile for data export. Reason: " << device->lastResultText() << endl;
	}
	else if (!device->startExportRecording(recordingIndex))
	{
		cout << "Could not export first recording. Reason: " << device->lastResultText() << endl;
	}
	else
	{
		// Sleeping for max 10 seconds...
		int64_t startTime = XsTime::timeStampNow();
		while (!xdpcHandler.exportDone() && XsTime::timeStampNow() - startTime <= 10000)
			XsTime_msleep(100);

		if (xdpcHandler.exportDone())
		{
			cout << "File export finished!" << endl;
		}
		else
		{
			cout << "Done sleeping, aborting export for demonstration purposes." << endl;

			if (!device->stopExportRecording())
				cout << "Device stop export failed. Reason: " << device->lastResultText() << endl;
			else
				cout << "Device export stopped." << endl;
		}

		cout << "Received " << xdpcHandler.packetsReceived() << " data packets from the recording." << endl;
	}

	device->disableLogging();

	xdpcHandler.cleanup();

	return 0;
}
