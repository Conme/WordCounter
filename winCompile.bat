@ECHO OFF
set conf=%1

if "%conf%"=="Debug" set Valid=1
if "%conf%"=="Release" set Valid=1
if defined Valid (
    mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=%conf% ..
	MSBuild WordCounter.sln /property:Configuration=%conf%
	cd %conf%
) else (
    echo "Invalid argument for Compilation Configuration. Enter Debug or Release" 
) 
