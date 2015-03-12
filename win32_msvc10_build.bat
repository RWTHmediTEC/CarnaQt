@echo off

echo ############################################################
echo ### Build script for installation inside MEDITEC_LIBS    ###
echo ###                                                      ###
echo ### Specify command line parameters for build options:   ###
echo ###    "-DBUILD_DEMO=ON"     Build interactive demo      ###
echo ###    "-DBUILD_TEST=ON"     Build unit tests            ###
echo ###    "-DBUILD_DOC=ON"      Build API documentation     ###
echo ###                                                      ###
echo ### !! You must put all parameters in quotation marks !! ###
echo ############################################################
timeout /t 5

call "%VS100COMNTOOLS%\vsvars32.bat"

mkdir build
mkdir build\nmake_debug
mkdir build\nmake_release
mkdir build\VisualStudio2010

cd build\nmake_debug
cmake -G"NMake Makefiles" %1 %2 %3 -DCMAKE_INSTALL_PREFIX="%MEDITEC_LIBS%" ..\..
nmake
nmake install

cd ..\nmake_release
cmake -G"NMake Makefiles" %1 %2 %3 -DCMAKE_INSTALL_PREFIX="%MEDITEC_LIBS%" -DBUILD_DOC=OFF -DCMAKE_BUILD_TYPE=Release ..\..
nmake
nmake install

cd ..\VisualStudio2010
cmake -G"Visual Studio 10" -DCMAKE_INSTALL_PREFIX="%MEDITEC_LIBS%" -DBUILD_TEST=ON -DBUILD_DEMO=ON ..\..

cd ..\..