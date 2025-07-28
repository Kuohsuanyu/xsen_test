# Running an example
Below we describe the steps to run the 64 bit Example on Windows 10 64 bit

 1. Open the `csharp.sln` using Microsoft Visual Studio
 2. Make sure the dependencies for your platform are there
    1. By default these are expected to be in `$(ProjectDir)..\..\wrap_csharp$(PlatformArchitecture)\` but this can of course be adjusted
        in the project settings. For Windows 64 for example $(ProjectDir)..\..\wrap_csharp64\
	2. The `extralibs` folder contains the required libaries.
 3. Select the Configurations, for example Release and x64.
 4. Build and Run one of the Movella DOT CSharp Examples
