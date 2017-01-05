echo off
FOR %%I IN (.) DO SET CurrDirName=%%~nI%%~xI
where /q cmake
IF %ERRORLEVEL%==1 (
    Echo cmake not in path
	SET p86=C:\Program Files (x86)
	PATH=%path%;%ProgramFiles(x86)%\CMake\bin
	PATH=%path%;%ProgramFiles%\\CMake\bin
 )
where /q cmake
IF %ERRORLEVEL%==1 (
    Echo Couldn't find cmake.exe
) ELSE ( 
   echo Cmake building for VS2015 Win64
   cd ..
   mkdir %CurrDirName%_build
   cd %CurrDirName%_build
   cmake -G "Visual Studio 14 2015 Win64" ../%CurrDirName%/
   echo cmake complete, open the sln in the build dir
 )
pause > nul