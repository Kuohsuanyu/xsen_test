
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

import java.util.ArrayList;

public class DotExampleSynchronization {

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
        // Make sure all connected devices have the same filter profile and outputrate
        if (device.setOnboardFilterProfile(new XsString("General")))
          System.out.println("Successfully set filter profile to General");
        else
          System.out.println("Failed to set filter profile!");

        if (device.setOutputRate(20))
          System.out.println("Successfully set output rate to 20 Hz");
        else
          System.out.println("Failed to set output rate!");
      }

      XsDotConnectionManager manager = xdpcHandler.manager();
      ArrayList<XsDotDevice> deviceList = xdpcHandler.connectedDots();
      System.out.println(String.format("\nStarting sync for connected devices... Root node: %s", deviceList.get(deviceList.size() - 1).bluetoothAddress()));
      System.out.println("This takes at least 14 seconds");
      if (!manager.startSync(deviceList.get(deviceList.size() - 1).bluetoothAddress()))
      {
        System.out.println(String.format("Could not start sync. Reason: %s", manager.lastResultText().toString()));
        if (manager.lastResult() != XsResultValue.XRV_SYNC_COULD_NOT_START)
        {
          System.out.println("Sync could not be started. Aborting.");
          xdpcHandler.cleanup();
          System.exit(-1);
        }

        // If (some) devices are already in sync mode. Disable sync on all devices first.
        manager.stopSync();
        System.out.println("Retrying start sync after stopping sync");
        if (!manager.startSync(deviceList.get(deviceList.size() - 1).bluetoothAddress()))
        {
          System.out.println(String.format("Could not start sync. Reason: %s. Aborting.", manager.lastResultText().toString()));
          xdpcHandler.cleanup();
          System.exit(-1);
        }
      }

      // Start live data output. Make sure root node is last to go to measurement.
      System.out.println("Putting devices into measurement mode: ");
      for (XsDotDevice device : deviceList)
        if (!device.startMeasurement(XsPayloadMode.ExtendedEuler))
        {
          System.out.println(String.format("Could not put device into measurement mode. Reason: %s", device.lastResultText().toString()));
          continue;
        }

      System.out.println("Starting measurement...");

      System.out.println("Main loop. Measuring data for 2 seconds.");
      System.out.println("-----------------------------------------");

      for (XsDotDevice device : deviceList)
        System.out.print(String.format("%-27s", device.bluetoothAddress()));
      System.out.print("\n");

      long startTime = XsTimeStamp.nowMs();
      while (XsTimeStamp.nowMs() - startTime <= 2000)
      {
        if (xdpcHandler.packetsAvailable())
        {
          for (XsDotDevice device : deviceList)
          {
            // Retrieve a packet
            XsDataPacket packet = xdpcHandler.getNextPacket(device.bluetoothAddress().toString());

            if (packet.containsOrientation())
            {
              XsEuler euler = packet.orientationEuler();
              System.out.print(String.format("TS:%8d, Roll:%7.2f| ", packet.sampleTimeFine(), euler.roll()));
            }

            packet.delete();
          }
          System.out.print("\n");
        }
      }
      System.out.println("\n-----------------------------------------");

      System.out.println("Stopping measurement...");
      for (XsDotDevice device : deviceList) {
        if (!device.stopMeasurement())
          System.out.print("Failed to stop measurement.");
      }

      System.out.println("Stopping sync...");
      if (!manager.stopSync())
          System.out.print("Failed to stop sync.");

      xdpcHandler.cleanup();
    }
}
