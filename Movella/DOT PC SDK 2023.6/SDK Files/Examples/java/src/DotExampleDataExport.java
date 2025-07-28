
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

import com.movella.movelladot_pc_sdk.*;

public class DotExampleDataExport{
    static{
      try {
        // Make sure to add the movelladot_pc_sdk DLL to the PATH environment variable / Classpath so Java can find it
        System.loadLibrary("movelladot_pc_sdk_java64");
        System.out.println("Loaded Library");
      } catch (UnsatisfiedLinkError e) {
        System.err.println("Native code library failed to load.\n" + e);
        System.exit(1);
      }
    }

    public static void main(String[] args) throws Exception {

      XdpcHandler xdpcHandler = new XdpcHandler();

      if (!xdpcHandler.initialize())
        System.exit(-1);

      xdpcHandler.detectUsbDevices();

      if (xdpcHandler.detectedDots().empty())
      {
        System.out.println("No Movella DOT device(s) found. Aborting.");
        System.exit(-1);
      }

      xdpcHandler.connectDots();

      if (xdpcHandler.connectedUsbDots().isEmpty())
      {
        System.out.println("Could not connect to any Movella DOT device(s). Aborting.");
        System.exit(-1);
      }

      XsIntArray exportData = new XsIntArray();
      exportData.push_back(XsRecordingExportDataField.RecordingData_Timestamp);
      exportData.push_back(XsRecordingExportDataField.RecordingData_Euler);
      exportData.push_back(XsRecordingExportDataField.RecordingData_Acceleration);
      exportData.push_back(XsRecordingExportDataField.RecordingData_AngularVelocity);
      exportData.push_back(XsRecordingExportDataField.RecordingData_MagneticField);
      exportData.push_back(XsRecordingExportDataField.RecordingData_Status);

      XsDotUsbDevice device = xdpcHandler.connectedUsbDots().get(0);

      int recordingIndex = device.recordingCount();
      XsRecordingInfo recInfo = device.getRecordingInfo(recordingIndex);
      if (recInfo.empty())
      {
        System.out.println(String.format("Could not get recording info. Reason: %s", device.lastResultText().toString()));
      }
      else
      {
        System.out.println(String.format("Recording [%d], Storage Size: %d bytes", recordingIndex, recInfo.storageSize()));
        System.out.println(String.format("Recording [%d], Recording time: %d seconds", recordingIndex, recInfo.totalRecordingTime()));
      }

      String csvFilename = String.format("device_%s_%d.csv", device.deviceId().toXsString().toString(), recordingIndex);
      System.out.println(String.format("Exporting recording %d to file %s", recordingIndex, csvFilename));

      if (!device.selectExportData(exportData))
      {
        System.out.println(String.format("Could not select export data. Reason: %s", device.lastResultText().toString()));
      }
      else if (!device.enableLogging(new XsString(csvFilename)))
      {
        System.out.println(String.format("Could not open logfile for data export. Reason: %s", device.lastResultText().toString()));
      }
      else if (!device.startExportRecording(recordingIndex))
      {
        System.out.println(String.format("Could not export recording. Reason: %s", device.lastResultText().toString()));
      }
      else
      {
        // Sleep for max 10 seconds...
        long startTime = XsTimeStamp.nowMs();
        while(!xdpcHandler.exportDone() && XsTimeStamp.nowMs() - startTime <= 10000)
          Thread.sleep(100);

        if (xdpcHandler.exportDone())
        {
          System.out.println("File export finished!");
        }
        else
        {
          System.out.println("Done sleeping, aborting export for demonstration purposes.");

          if (!device.stopExportRecording())
            System.out.println(String.format("Device stop export failed. Reason: %s", device.lastResultText().toString()));
          else
            System.out.println("Device export stopped.");
        }

        System.out.println(String.format("Received %d data packets from the recording.", xdpcHandler.packetsReceived()));
      }

      device.disableLogging();

      xdpcHandler.cleanup();
    }
}
