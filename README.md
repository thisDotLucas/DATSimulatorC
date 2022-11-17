# DATSimulatorC

## Build using cmake

Download latest version of cmake: https://cmake.org/download/<br />
In the instruction examples I've used vcpkg as the package manager: https://vcpkg.io/en/getting-started.html<br />
If you want to use some other generator than specified in the examples: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html

### Windows

#### Command line
Run the following command:<br />
```
cmake -DCMAKE_TOOLCHAIN_FILE=\<Path to your vcpkg folder>\scripts\buildsystems\vcpkg.cmake 
-S <Path to the root directory> -B <Path to your build folder> -G "Visual Studio 17 2022"
```
Now in your specified build directory you can use msbuild to build the executables.<br />
```
msbuild <Path to your DATSimulatorC.sln file>
```

Now the executable can be found in the Debug folder under the DATSimulatorC folder.<br />
The unit test executable is under the Tests folder.

#### Visual Studio IDE
See here: https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170<br />
Note the vcpkg integration section.

### Linux
Work in progress...<br />

### MacOs
Work in progress...<br />

