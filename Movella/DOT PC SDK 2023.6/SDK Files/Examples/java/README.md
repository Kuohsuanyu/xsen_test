## Setting up the Movella DOT PC SDK Java build
 1. The example is structured as a Visual Studio Code Java Project but this is of course just an example where you could just as well use javac and java directly to compile and run. ( Also see: https://code.visualstudio.com/docs/java/java-tutorial) 
 2. Make sure to load the required Library
	- Add the library folder to your PATH environment variable and use `System.loadLibrary("movelladot_pc_sdk_java64");`
	- Java needs to be able to find it on the classpath or you will get errors that dependent libraries can not be found
	- The library files are located in the 'java' folder.
        - Visual Studio Code; Opening the project from the java folder makes sure the files are in the current directory.
        - Linux shell: Setting LD_LIBRARY_PATH to the java folder is an option to make sure the libraries can be found.
 3. Build and Run the 'DotExampleReceiveData' or 'DotExampleWrite'

## From a terminal on Windows/Linux
Alternatively, if you wish to run the examples without VS Code you can use the following commands:
- cd .\src\
- javac .\DotExampleReceiveData.java .\com\movella\movelladot_pc_sdk\*.java
- cd ..
- java -D"java.library.path"=. -cp .\src\ DotExampleReceiveData

**Note**: If you encounter VS Code warning you of build problems ignore this False Positive, click Proceed and it should just work.
