
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
// Movella DOT SDK C++ example DOT synchronization.
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
		return -1;
	}

	xdpcHandler.connectDots();

	if (xdpcHandler.connectedDots().empty())
	{
		cout << "Could not connect to any Movella DOT device(s). Aborting." << endl;
		return -1;
	}

	for (auto& device : xdpcHandler.connectedDots())
	{
		// Make sure all connected devices have the same filter profile and outputrate
		if (device->setOnboardFilterProfile(XsString("General")))
			cout << "Successfully set profile to General" << endl;
		else
			cout << "Setting filter profile failed!" << endl;

		if (device->setOutputRate(20))
			cout << "Successfully set output rate to 20 Hz" << endl;
		else
			cout << "Setting output rate failed!" << endl;
	}

	XsDotConnectionManager* manager = xdpcHandler.manager();
	list<XsDotDevice*> deviceList = xdpcHandler.connectedDots();
	cout << endl << "Starting sync for connected devices... Root node: " << deviceList.back()->bluetoothAddress() << endl;
	cout << "This takes at least 14 seconds" << endl;
	if (!manager->startSync(deviceList.back()->bluetoothAddress()))
	{
		cout << "Could not start sync. Reason: " << manager->lastResultText() << endl;
		if (manager->lastResult() != XRV_SYNC_COULD_NOT_START)
		{
			cout << "Sync could not be started. Aborting." << endl;
			xdpcHandler.cleanup();
			return -1;
		}

		// If (some) devices are already in sync mode. Disable sync on all devices first.
		manager->stopSync();
		cout << "Retrying start sync after stopping sync" << endl;
		if (!manager->startSync(deviceList.back()->bluetoothAddress()))
		{
			cout << (XsString() << "Could not start sync. Reason: " << manager->lastResultText() << "Aborting.").c_str() << endl;
			xdpcHandler.cleanup();
			return -1;
		}
	}

	// Start live data output. Make sure root node is last to go to measurement.
	cout << "Putting devices into measurement mode." << endl;
	for (auto& device : xdpcHandler.connectedDots())
	{
		if (!device->startMeasurement(XsPayloadMode::OrientationEuler))
		{
			cout << "Could not put device into measurement mode. Reason: " << device->lastResultText() << endl;
			continue;
		}
	}

	cout << "\nMain loop. Logging data for 2 seconds." << endl;
	cout << string(83, '-') << endl;

	// First printing some headers so we see which data belongs to which device
	for (auto const& device : xdpcHandler.connectedDots())
		cout << setw(27) << left << device->portInfo().bluetoothAddress();
	cout << endl;

	uint64_t startTime = XsTime::timeStampNow();
	while (XsTime::timeStampNow() - startTime <= 2000)
	{
		if (xdpcHandler.packetsAvailable())
		{
			for (auto const& device : xdpcHandler.connectedDots())
			{
				// Retrieve a packet
				XsDataPacket packet = xdpcHandler.getNextPacket(device->bluetoothAddress());

				if (packet.containsOrientation())
				{
					XsEuler euler = packet.orientationEuler();

					cout << "TS:" << right << setw(8) << fixed << packet.sampleTimeFine()
						<< ", Roll:" << right << setw(7) << fixed << setprecision(2) << euler.roll()
						<< "| ";
				}
			}
			cout << endl;
		}
		XsTime::msleep(0);
	}
	cout << "\n" << string(83, '-') << "\n";
	cout << endl;

	cout << "Stopping measurement..." << endl;
	for (auto device : xdpcHandler.connectedDots())
	{
		if (!device->stopMeasurement())
			cout << "Failed to stop measurement.";
	}

	cout << "Stopping sync..." << endl;
	if (!manager->stopSync())
		cout << "Failed to stop sync.";

	xdpcHandler.cleanup();

	return 0;
}
