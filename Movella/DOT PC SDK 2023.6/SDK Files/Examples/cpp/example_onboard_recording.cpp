
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
// Movella DOT SDK C++ example DOT onboard recording.
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
		cout << "Set onboard recording data rate to 60 Hz" << endl;
		if (device->setOutputRate(60))
			cout << "Successfully set onboard recording rate" << endl;
		else
			cout << "Setting onboard recording rate failed!" << endl;

		xdpcHandler.resetRecordingStopped();

		cout << endl;
		cout << "Starting timed onboard recording for 10 seconds." << endl;
		if (!device->startTimedRecording(10))
		{
			cout << "Could not start onboard recording. Reason: " << device->lastResultText() << endl;
			continue;
		}

		do
		{
			XsRecordingTimeInfo recordingTimeInfo = device->getRecordingTime();
			XsTimeStamp ts((int64_t)recordingTimeInfo.startUTC() * 1000);
			cout << "\rRecording start time: " <<
				ts.utcToLocalTime().toString().toStdString() <<
				" total time: " <<
				recordingTimeInfo.totalRecordingTime() << " seconds" <<
				" remaining time: " <<
				recordingTimeInfo.remainingRecordingTime() << " seconds";
			cout << flush;
			XsTime::msleep(1000);
		} while (!xdpcHandler.recordingStopped());
		cout << endl;
	}

	xdpcHandler.cleanup();

	return 0;
}
