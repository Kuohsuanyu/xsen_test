
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
using System.Collections.Generic;
using MovellaDotPcSdk;
using Movella;

namespace ExampleSynchronization
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
                // Make sure all connected devices have the same filter profile and outputrate
                if (device.setOnboardFilterProfile(new XsString("General")))
                    Console.WriteLine("Successfully set filter profile to General");
                else
                    Console.WriteLine("Failed to set filter profile!");

                if (device.setOutputRate(20))
                    Console.WriteLine("Successfully set output rate to 20 Hz");
                else
                    Console.WriteLine("Failed to set output rate!");
            }

            XsDotConnectionManager manager = xdpcHandler.Manager;
            List<XsDotDevice> deviceList = xdpcHandler.ConnectedDots;
            Console.WriteLine("\nStarting sync for connected devices... Root node: {0}", deviceList[deviceList.Count - 1].bluetoothAddress().toString());
            Console.WriteLine("This takes at least 14 seconds");
            if (!manager.startSync(deviceList[deviceList.Count - 1].bluetoothAddress()))
            {
                Console.WriteLine("Could not start sync. Reason: {0}", manager.lastResultText().toString());
                if (manager.lastResult() != XsResultValue.XRV_SYNC_COULD_NOT_START)
                {
					Console.WriteLine("Sync could not be started. Aborting.");
					xdpcHandler.cleanup();
					return;
				}

				// If (some) devices are already in sync mode. Disable sync on all devices first.
				manager.stopSync();
				Console.WriteLine("Retrying start sync after stopping sync");
				if (!manager.startSync(deviceList[deviceList.Count - 1].bluetoothAddress()))
				{
					Console.WriteLine("Could not start sync. Reason: {0}. Aborting.", manager.lastResultText().toString());
					xdpcHandler.cleanup();
					return;
				}
            }

            // Start live data output. Make sure root node is last to go to measurement.
            Console.WriteLine("Putting devices into measurement mode. ");
            foreach (XsDotDevice device in xdpcHandler.ConnectedDots)
            {
                if (!device.startMeasurement(XsPayloadMode.ExtendedEuler))
                {
                    Console.WriteLine("Could not put device into measurement mode. Reason: {0}", device.lastResultText().toString());
                    continue;
                }
            }

            Console.WriteLine("Starting measurement...");

            Console.WriteLine("Main loop. Measuring data for 2 seconds.");
            Console.WriteLine("-----------------------------------------");

            // First printing some headers so we see which data belongs to which device
            foreach (XsDotDevice device in xdpcHandler.ConnectedDots)
                Console.Write("{0,-27}", device.bluetoothAddress().toString());
            Console.Write("\n");

            long startTime = XsTimeStamp.nowMs();
            while (XsTimeStamp.nowMs() - startTime <= 2000)
            {
                if (xdpcHandler.packetsAvailable())
                {
                    foreach (XsDotDevice device in xdpcHandler.ConnectedDots)
                    {
                        // Retrieve a packet
                        XsDataPacket packet = xdpcHandler.getNextPacket(device.bluetoothAddress().toString());

                        if (packet.containsOrientation())
                        {
                            XsEuler euler = packet.orientationEuler();
                            Console.Write("TS:{0,8}, Roll:{1,7:f2}| ", packet.sampleTimeFine(), euler.roll());
                        }

                        packet.Dispose();
                    }
                    Console.WriteLine("");
                }
            }
            Console.WriteLine("\n-----------------------------------------");

            Console.WriteLine("Stopping measurement...");
            foreach (XsDotDevice device in xdpcHandler.ConnectedDots)
            {
                if (!device.stopMeasurement())
                    Console.Write("Failed to stop measurement.");
            }

            Console.WriteLine("Stopping sync...");
            if (!manager.stopSync())
                Console.Write("Failed to stop sync.");

            xdpcHandler.cleanup();
        }
    }
}
