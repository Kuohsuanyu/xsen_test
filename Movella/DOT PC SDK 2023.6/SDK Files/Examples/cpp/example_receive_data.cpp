
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
// Movella DOT SDK C++ example DOT receive data.
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

	xdpcHandler.scanForDots();

	if (xdpcHandler.detectedDots().empty())
	{
		cout << "No Movella DOT device(s) found. Aborting." << endl;
		xdpcHandler.cleanup();
		return -1;
	}

	xdpcHandler.connectDots();

	if (xdpcHandler.connectedDots().empty())
	{
		cout << "Could not connect to any Movella DOT device(s). Aborting." << endl;
		xdpcHandler.cleanup();
		return -1;
	}

	for (auto& device : xdpcHandler.connectedDots())
	{		
		auto filterProfiles = device->getAvailableFilterProfiles();
		cout << filterProfiles.size() << " available filter profiles:" << endl;
		for (auto& f : filterProfiles)
			cout << f.label() << endl;

		cout << "Current profile: " << device->onboardFilterProfile().label() << endl;
		if (device->setOnboardFilterProfile(XsString("General")))
			cout << "Successfully set profile to General" << endl;
		else
			cout << "Setting filter profile failed!" << endl;

		cout << "Setting quaternion CSV output" << endl;
		device->setLogOptions(XsLogOptions::Quaternion);

		XsString logFileName = XsString("logfile_") << device->bluetoothAddress().replacedAll(":", "-") << ".csv";
		cout << "Enable logging to: " << logFileName.c_str() << endl;
		if (!device->enableLogging(logFileName))
			cout << "Failed to enable logging. Reason: " << device->lastResultText() << endl;

		cout << "Putting device into measurement mode." << endl;
		if (!device->startMeasurement(XsPayloadMode::ExtendedEuler))
		{
			cout << "Could not put device into measurement mode. Reason: " << device->lastResultText() << endl;
			continue;
		}
	}

	cout << "\nMain loop. Logging data for 10 seconds." << endl;
	cout << string(83, '-') << endl;

	// First printing some headers so we see which data belongs to which device
	for (auto const& device : xdpcHandler.connectedDots())
		cout << setw(42) << left << device->bluetoothAddress();
	cout << endl;

	bool orientationResetDone = false;
	int64_t startTime = XsTime::timeStampNow();
	while (XsTime::timeStampNow() - startTime <= 10000)
	{
		if (xdpcHandler.packetsAvailable())
		{
			cout << "\r";
			for (auto const& device : xdpcHandler.connectedDots())
			{
				// Retrieve a packet
				XsDataPacket packet = xdpcHandler.getNextPacket(device->bluetoothAddress());

				if (packet.containsOrientation())
				{
					XsEuler euler = packet.orientationEuler();

					cout << "Roll:" << right << setw(7) << fixed << setprecision(2) << euler.roll()
						<< ", Pitch:" << right << setw(7) << fixed << setprecision(2) << euler.pitch()
						<< ", Yaw:" << right << setw(7) << fixed << setprecision(2) << euler.yaw()
						<< "| ";
				}
			}
			cout << flush;
			if (!orientationResetDone && (XsTime::timeStampNow() - startTime) > 5000)
			{
				for (auto const& device : xdpcHandler.connectedDots())
				{
					cout << endl << "Resetting heading for device " << device->bluetoothAddress() << ": ";
					if (device->resetOrientation(XRM_Heading))
						cout << "OK";
					else
						cout << "NOK: " << device->lastResultText();
				}
				cout << endl;
				orientationResetDone = true;
			}
		}
		XsTime::msleep(0);
	}
	cout << "\n" << string(83, '-') << "\n";
	cout << endl;

	for (auto const& device : xdpcHandler.connectedDots())
	{
		cout << endl << "Resetting heading to default for device " << device->bluetoothAddress() << ": ";
		if (device->resetOrientation(XRM_DefaultAlignment))
			cout << "OK";
		else
			cout << "NOK: " << device->lastResultText();
	}
	cout << endl << endl;

	cout << "Stopping measurement..." << endl;
	for (auto device : xdpcHandler.connectedDots())
	{
		if (!device->stopMeasurement())
			cout << "Failed to stop measurement.";

		if (!device->disableLogging())
			cout << "Failed to disable logging.";
	}

	xdpcHandler.cleanup();

	return 0;
}
