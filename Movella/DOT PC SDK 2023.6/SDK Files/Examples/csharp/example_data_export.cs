
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

namespace ExampleDataExport
{
    static class Program
    {
        static void Main()
        {
            XdpcHandler xdpcHandler = new XdpcHandler();

            if (!xdpcHandler.initialize())
                return;

            xdpcHandler.detectUsbDevices();

            if (xdpcHandler.detectedDots().empty())
            {
                Console.WriteLine("No Movella DOT device(s) found. Aborting.");
                xdpcHandler.cleanup();
                return;
            }

            xdpcHandler.connectDots();

            if (xdpcHandler.ConnectedUsbDots.Count == 0)
            {
                Console.WriteLine("Could not connect to any Movella DOT device(s). Aborting.");
                xdpcHandler.cleanup();
                return;
            }

            XsIntArray exportData = new XsIntArray();
            exportData.push_back(XsRecordingExportDataField.RecordingData_Timestamp);
            exportData.push_back(XsRecordingExportDataField.RecordingData_Euler);
            exportData.push_back(XsRecordingExportDataField.RecordingData_Acceleration);
            exportData.push_back(XsRecordingExportDataField.RecordingData_AngularVelocity);
            exportData.push_back(XsRecordingExportDataField.RecordingData_MagneticField);
            exportData.push_back(XsRecordingExportDataField.RecordingData_Status);

            XsDotUsbDevice device = xdpcHandler.ConnectedUsbDots[0];

            int recordingIndex = device.recordingCount();
            XsRecordingInfo recInfo = device.getRecordingInfo(recordingIndex);
            if (recInfo.empty())
            {
                Console.WriteLine("Could not get recording info. Reason: {0}", device.lastResultText().toString());
            }
            else
            {
                Console.WriteLine("Recording [{0}], Storage Size: {1} bytes", recordingIndex, recInfo.storageSize());
                Console.WriteLine("Recording [{0}], Recording Time: {1} seconds", recordingIndex, recInfo.totalRecordingTime());
            }

            string csvFilename = String.Format("device_{0}_{1}.csv", device.deviceId().toXsString().toString(), recordingIndex);
            Console.WriteLine("Exporting recording {0} to file {1}", recordingIndex, csvFilename);

            if (!device.selectExportData(exportData))
            {
                Console.WriteLine("Could not select export data. Reason: {0}", device.lastResultText().toString());
            }
            else if (!device.enableLogging(new XsString(csvFilename)))
            {
                Console.WriteLine("Could not open logfile for data export. Reason: {0}", device.lastResultText().toString());
            }
            else if (!device.startExportRecording(recordingIndex))
            {
                Console.WriteLine("Could not export first recording. Reason: {0}", device.lastResultText().toString());
            }
            else
            {
                // Sleeping for max 10 seconds...
                long startTime = XsTimeStamp.nowMs();
                while (!xdpcHandler.ExportDone && XsTimeStamp.nowMs() - startTime <= 10000)
                    System.Threading.Thread.Sleep(100);

                if (xdpcHandler.ExportDone)
                {
                    Console.WriteLine("File export finished!");
                }
                else
                {
                    Console.WriteLine("Done sleeping, aborting export for demonstration purposes.");

                    if (!device.stopExportRecording())
                        Console.WriteLine("Device stop export failed. Reason: {0}", device.lastResultText().toString());
                    else
                        Console.WriteLine("Device export stopped.");
                }

                Console.WriteLine("Received {0} data packets from the recording.", xdpcHandler.PacketsReceived);
            }

            device.disableLogging();

            xdpcHandler.cleanup();
        }
    }
}
