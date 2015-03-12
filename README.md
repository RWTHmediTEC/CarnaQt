Carna
========

Carna is a lightweight library for simple and fast visualization of CT data.
It is named after the greek god of organs (yup, they really did have even one for organs).

Go to: [Library Documentation](https://rwthmeditec.github.io/Carna/)

<img src="https://rwthmeditec.github.io/Carna/XRay01.png">

---
## Contents

* [Dependencies](#1-dependencies)
* [Build instructions](#2-build-instructions)
	* [Creating Visual Studio project files](#21-creating-visual-studio-project-files)
	* [Building directly](#22-building-directly)
	* [Installation notes](#23-installation-notes)
	* [The MediTEC-way](#24-the-meditec-way)
* [Including in your project](#3-including-in-your-project)
	* [The CMake-way](#31-the-cmake-way)
	* [The MediTEC-way](#32-the-meditec-way)
	* [Manually](#33-manually)
* [See also](#4-see-also)
 
---
## 1. Dependencies

* [Eigen](http://eigen.tuxfamily.org/) ≥ 3.0.5
* OpenGL 3.3
* GLEW ≥ 1.7

Compilation process has been tested with following tool chains:

* **GCC 4.6** works but is known to have a bug in the C++11 regex
  module that prevents the test suite from operating properly.
* **GCC 4.9:** is known to be fully supported.
* **Visual Studio 2010** is known to be fully supported.

---
## 2. Build instructions

The default build process requires CMake ≥ 3.0.2.

This section explains three ways of building Carna:

1. Creating Visual Studio project files and building it from the IDE
2. Building Carna directly through CMake from command line
3. If you are a colleague from MediTEC, you can use the batch script.

Regardless which build method you pick,
first thing you need to do is to fetch the latest stable version.
If you are using git from command line,
you can simply run following command
from within the directory where you want to download the sources:

```bat
git clone https://github.com/RWTHmediTEC/Carna.git
```
    
After a few seconds there should be a new folder named `Carna`
at your current working directory.

### 2.1. Creating Visual Studio project files

First create the directory where the Visual Studio files should go to:

```bat
cd Carna

mkdir build
mkdir build\VisualStudio2010

cd build\VisualStudio2010
```
    
Then initialize the Visual Studio environment:

```bat
call "%VS100COMNTOOLS%\vsvars32.bat"
```
    
And finnaly invoke CMake like this:

```bat
cmake -G"Visual Studio 10" -DCMAKE_INSTALL_PREFIX="C:\Libs" ..\..
```
    
You may also leave out the `-DCMAKE_INSTALL_PREFIX` parameter
if you do not want to specify any particular installation directory.
In this case the default installation directory will be set,
which is the value of the environmental variable `%ProgramFiles%` on Windows.

At this point the Visual Studio project files are ready.
You can proceed by opening the solution file `Carna.sln`
that was created in `Carna\build\VisualStudio2010`.
Note that building the `INSTALL` project from the solution
actually triggers the installation routine
to the destination you configured via `-DCMAKE_INSTALL_PREFIX`.

### 2.2. Building directly

The first step is to create the directories
where the results of the building process will be stored.
You can use any names you like for the directories,
it's only important to distinguish between "debug" and "release" files:

```bat
cd Carna

mkdir build
mkdir build\debug
mkdir build\release
```

Then initialize the building enviroment.
Use the command below if you are going to use Visual Studio for compilation:

```bat
call "%VS100COMNTOOLS%\vsvars32.bat"
```
    
Now it's time to run the build process.
Lets build the "debug" version first:

```bat
cd build\debug
cmake -G"NMake Makefiles" -DCMAKE_INSTALL_PREFIX="C:\Libs" -DBUILD_DOC=OFF ..\..
nmake
```
    
You may also leave out the `-DCMAKE_INSTALL_PREFIX` parameter
if you do not want to specify any particular installation directory.
In this case the default installation directory will be set,
which is the value of the environmental variable `%ProgramFiles%` on Windows.

The parameter `-DBUILD_DOC=OFF` forces CMake to skip running Doxygen, if it is installed at all. The idea is to build the documentation only once. Since we are going to build the "release" version next, leaving out this parameter at this point would lead to Doxygen being triggered twice.

Run `nmake install` now if you wish to install the "debug" version on your system.

If everything went well,
run the build for the "release" version:

```bat
cd ..\nmake_release
cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..\..
nmake
```

Run `nmake install` (again) if you want to install the "release" version.

### 2.3. Installation notes

You may need finer control of the installation routines' destinations.
Regardless which of the methods presented above you choose,
you can specify the destinations for the
headers, the library and the generated CMake files
by passing particular parameters to `cmake`:

* `INSTALL_CMAKE_DIR` specifies where the `FindCarna.cmake` file goes to.
* `INSTALL_LIBRARY_DIR` specifies where the built binary files go to.
* `INSTALL_HEADERS_DIR` specifies where the headers are going to installed to.

If you use relative paths for these parameters,
they will be resolved relatively to `CMAKE_INSTALL_PREFIX`.
Consider using absolute paths if you don't want that.

### 2.4. The MediTEC-way

Make sure you have the environmental variable `%MEDITEC_LIBS%` set
and it is poiting to an existing directory that you have permissions to write to,
than

* run `create_msvc10_project.bat` to create the Visual Studio project files, or
* run `win32_msvc10_build.bat` to build and install the whole package.

---
## 3. Including in your project

It is assumed at this point that you either have built and installed Carna with CMake,
or you have fetched the binaries and the corresponding headers from somewhere.

### 3.1. The CMake-way

Add a `find_package` directive to your project's `CMakeLists.txt` file, e.g.:

```CMake
find_package( Carna REQUIRED )
include_directories( ${CARNA_INCLUDE_DIR} )
```

If you need to put a constraint on the version, use `find_package(Carna 2.5.0 REQUIRED)`
to pick a package with a version *compatible* to 2.5.0,
or use `find_package(Carna 2.5.0 EXACT REQUIRED)` to pick a package by the exact version.

You also need to add the headers (usually *only* the headers) from TRTK and Eigen:

```CMake
# Eigen
find_package( Eigen3 3.0.5 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

# TRTK
find_package( TRTK 0.13.1 REQUIRED )
include_directories( ${TRTK_INCLUDE_DIR} )
```

Finally add Carna to the linking stage:

```CMake
target_link_libraries( ${TARGET_NAME} ${SOME_OTHER_LIBRARIES} ${CARNA_LIBRARIES} )
```

This method relies on CMake being able to locate the proper `FindCarna.cmake` file.
If you've built Carna from source,
than you have determined it's location either through `CMAKE_INSTALL_PREFIX`
or `INSTALL_CMAKE_DIR` as described in ["installation notes"](#23-installation-notes).
You can specify the paths CMake searches for `FindCarna.cmake` by adjustung the
`CMAKE_MODULE_PATH` variable, e.g.:

```CMake
list( APPEND CMAKE_MODULE_PATH "C:/CMake/Modules" )
```

### 3.2 The MediTEC-way

If you are a colleague from MediTEC, you must also add the following line of code
*before* `find_package`, otherwise CMake will not find Carna:

```CMake
list(APPEND CMAKE_MODULE_PATH "${MEDITEC_LIBS}/Carna/2.5")
```

### 3.3. Manually

Find where your header files are located. You might look for `Carna.h` or `Version.h`.
Both of these files are contained by a directory named `Carna`.
Add the *parent* directory of the `Carna` directory,
that contains `Carna.h` and `Version.h` in turn,
to your project's include directories.

Then find the appropriate library file.
It's name depends on your platform and Carna version,
e.g. `Carna-2.5.0.lib` for the release and `Carna-2.5.0d.lib`
for the debug version respectively of Carna 2.5.0 on Windows.
Add both of these files to your project's linker stage.

---
## 4. See also

The additional [Carna-DICOM module](https://github.com/RWTHmediTEC/Carna-DICOM)
provides classes for loading DICOM data sets.
