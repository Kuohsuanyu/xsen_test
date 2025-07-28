
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

public class DotExampleReceiveData {

    static{
      try {
        // Make sure to add the movelladot_pc_sdk dll to the PATH environment variable / Classpath so Java can find it
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

      xdpcHandler.scanForDots();

      if (xdpcHandler.detectedDots().empty())
      {
        System.out.println("No Movella DOT device(s) found. Aborting.");
        System.exit(-1);
      }

      xdpcHandler.connectDots();

      if (xdpcHandler.connectedDots().isEmpty())
      {
        System.out.println("Could not connect to any Movella DOT device(s). Aborting.");
        System.exit(-1);
      }

      for (XsDotDevice device : xdpcHandler.connectedDots())
      {
        System.out.println("Available filter profiles: ");
        XsFilterProfileArray filterProfiles = device.getAvailableFilterProfiles();
        for (int j = 0; j < filterProfiles.size(); j++) {
          System.out.println(String.format("Found label: %s", filterProfiles.at(j).label().toString()));
        }

        System.out.println(String.format("Current filter profile: %s", device.onboardFilterProfile().label()));
        if (device.setOnboardFilterProfile(new XsString("General")))
          System.out.println("Successfully set filter profile to General");
        else
          System.out.println("Failed to set filter profile!");

        System.out.println("Setting quaternion CSV output");
		    device.setLogOptions(XsLogOptions.Quaternion);

        final XsString logFileName = new XsString(String.format("logfile_" + "%s" + ".csv", device.bluetoothAddress().toString().replace(':', '-')));
        System.out.println(String.format("Enable logging to: %s", logFileName));

        if (!device.enableLogging(logFileName))
          System.out.println(String.format("Failed to enable logging. Reason: %s", device.lastResultText().toString()));

        System.out.println("Putting device into measurement mode: ");
        if (!device.startMeasurement(XsPayloadMode.ExtendedEuler))
        {
          System.out.println(String.format("Could not put device into measurement mode. Reason: %s", device.lastResultText().toString()));
          continue;
        }
      }

      System.out.println("Starting measurement...");
                
      System.out.println("Main loop. Measuring data for 10 seconds.");
      System.out.println("-----------------------------------------");

      for (XsDotDevice device : xdpcHandler.connectedDots())
        System.out.print(String.format("%-42s", device.bluetoothAddress()));
      System.out.print("\n");

      boolean orientationResetDone = false;
      long startTime = XsTimeStamp.nowMs();
      while (XsTimeStamp.nowMs() - startTime <= 10000)
      {
        if (xdpcHandler.packetsAvailable())
        {
          System.out.print("\r");
          for (XsDotDevice device : xdpcHandler.connectedDots())
          {
            // Retrieve a packet
            XsDataPacket packet = xdpcHandler.getNextPacket(device.bluetoothAddress().toString());

            if (packet.containsOrientation())
            {
              XsEuler euler = packet.orientationEuler();
              System.out.print(String.format("Roll:%7.2f, Pitch:%7.2f, Yaw:%7.2f| ", euler.roll(), euler.pitch(), euler.yaw()));
            }

            packet.delete();
          }
          if (!orientationResetDone && (XsTimeStamp.nowMs() - startTime) > 5000)
          {
            for (XsDotDevice device : xdpcHandler.connectedDots())
            {
              System.out.print(String.format("\nResetting heading for device %s: ", device.bluetoothAddress().toString()));
              if (device.resetOrientation(XsResetMethod.XRM_Heading))
                System.out.print("OK");
              else
                System.out.print(String.format("NOK: %s", device.lastResultText().toString()));
            }
            System.out.print("\n");
            orientationResetDone = true;
          }
        }
      }
      System.out.println("\n-----------------------------------------");

      for (XsDotDevice device : xdpcHandler.connectedDots())
      {
        System.out.print(String.format("\nResetting heading for device %s: ", device.bluetoothAddress().toString()));
        if (device.resetOrientation(XsResetMethod.XRM_DefaultAlignment))
          System.out.print("OK");
        else
          System.out.print(String.format("NOK: %s", device.lastResultText().toString()));
      }
      System.out.println("\n");

      System.out.println("Stopping measurement...");
      for (XsDotDevice device : xdpcHandler.connectedDots()) {
        if (!device.stopMeasurement())
          System.out.print("Failed to stop measurement.");
        if (!device.disableLogging())
         System.out.print("Failed to disable logging.");
      }

      xdpcHandler.cleanup();
    }
}
