
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

﻿using System;
using MovellaDotPcSdk;
using Movella;

namespace ExampleOnboardRecording
{
	static class Program
	{
		static void Main()
		{
            XdpcHandler xdpcHandler = new XdpcHandler();

            if (!xdpcHandler.initialize())
                return;

            xdpcHandler.scanForDots();

            if (xdpcHandler.detectedDots().empty())
            {
                Console.WriteLine("No Movella DOT device(s) found. Aborting.");
                xdpcHandler.cleanup();
                return;
            }

            xdpcHandler.connectDots();

            if (xdpcHandler.ConnectedDots.Count == 0)
            {
                Console.WriteLine("Could not connect to any Movella DOT device(s). Aborting.");
                xdpcHandler.cleanup();
                return;
            }

            foreach (XsDotDevice device in xdpcHandler.ConnectedDots)
            {
                Console.WriteLine("Set onboard recording data rate to 60 Hz");
                if (device.setOutputRate(60))
                    Console.WriteLine("Successfully set onboard recording rate");
                else
                    Console.WriteLine("Setting onboard recording rate failed!");

                xdpcHandler.RecordingStopped = false;
                Console.WriteLine("");
                Console.WriteLine("Starting timed onboard recording for 10 seconds.");
                if (!device.startTimedRecording(10))
                {
                    Console.WriteLine("Could not start onboard recording. Reason: {0}", device.lastResultText().toString());
                    continue;
                }

                do
                {
                    XsRecordingTimeInfo recordingTimeInfo = device.getRecordingTime();
                    XsTimeStamp ts = new XsTimeStamp((long)recordingTimeInfo.startUTC() * 1000);
                    Console.Write("\rRecording start time: {0} total time: {1} seconds remaining time: {2} seconds", 
                        ts.utcToLocalTime().toXsString().toString(),
                        recordingTimeInfo.totalRecordingTime(),
                        recordingTimeInfo.remainingRecordingTime());
                    System.Threading.Thread.Sleep(1000);
                } while (!xdpcHandler.RecordingStopped);
            }

            xdpcHandler.cleanup();
        }
    }
}
