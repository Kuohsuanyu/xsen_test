
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

public class DotExampleFirmwareUpdate {

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
        System.out.println(String.format("Updating firmware for device: %s", device.deviceTagName().toString()));

        // Reset the update done flag to enable updating the next device in the example.
        xdpcHandler.resetUpdateDone();

        XsVersion firmwareFileVersion = device.startFirmwareUpdateFromServer();
        if (firmwareFileVersion.empty())
        {
          System.out.println(String.format("Did not find a new Firmware on the server, device result: %s", device.lastResultText().toString()));
          break;
        }
        System.out.println(String.format("Updating Firmware to version %s", firmwareFileVersion.toSimpleString()));
        while (!xdpcHandler.updateDone())
          Thread.sleep(100);
      }

      xdpcHandler.cleanup();
    }
}
