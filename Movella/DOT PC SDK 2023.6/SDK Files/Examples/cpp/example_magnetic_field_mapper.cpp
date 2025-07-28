
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
// Movella DOT SDK C++ example Magnetic Field Mapping.
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
		cout << "Starting magnetic field mapping for device " << device->deviceTagName() << " [" << device->bluetoothAddress() << "]" << endl;
		if (!device->startMagneticFieldMapping())
		{
			cout << "Could not start magnetic field mapping. Reason: " << device->lastResultText() << endl;
			continue;
		}

		// Add device to buffer to track MFM progress per device
		xdpcHandler.addDeviceToProgressBuffer(device->bluetoothAddress());
	}

	cout << "\nMain loop. Logging data till we reach 100 %." << endl;
	cout << string(83, '-') << endl;

	bool allDevicesDone = false;

	while (!allDevicesDone)
	{
		allDevicesDone = true;
		for (auto const& device : xdpcHandler.connectedDots())
		{
			if (xdpcHandler.progress(device->bluetoothAddress()) != 100)
			{
				allDevicesDone = false;
				break;
			}
		}

		xsYield();
	}
	cout << "\nAll devices done with Magnetic Field Mapping!" << endl;
	cout << "\n" << string(83, '-') << "\n";
	cout << endl;

	cout << "Stopping magnetic field mapping..." << endl;
	for (auto device : xdpcHandler.connectedDots())
	{
		if (!device->stopMagneticFieldMapping())
			cout << "Failed to stop magnetic field mapping.";
	}

	xdpcHandler.cleanup();

	return 0;
}
