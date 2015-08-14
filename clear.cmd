del Makefile
del SevenZipJBinding.cbp
del SevenZipJBinding.layout
del SevenZipJBinding.sdf
del SevenZipJBinding.opensdf
del SevenZipJBinding.sln
del sevenzipjbinding-Windows-x86.jar
del sevenzipjbinding-platforms.properties
del *.vcxproj
del *.vcxproj.filters
for %%i in (*.cmake) do if /I not "%%i"=="CTestConfig.cmake" del "%%i"
del *.tmp
del CMakeCache.txt
del DartConfiguration.tcl
del .vs\SevenZipJBinding\v14\.suo /F /AH
rmdir .vs /S /Q
rmdir CMakeFiles /S /Q
rmdir javac-test /S /Q
del jbinding-cpp\*.vcxproj
del jbinding-cpp\*.vcxproj.filters
del jbinding-cpp\SevenZipJBindingCPP.sln
del jbinding-cpp\cmake_install.cmake
del jbinding-cpp\SevenZipJBindingCPP.cbp
del jbinding-cpp\lib7-Zip-JBinding.dll.a
del jbinding-cpp\Makefile
del jbinding-cpp\lib7-Zip-JBinding.dll
rmdir jbinding-cpp\7-Zip-JBinding.dir /S /Q
rmdir jbinding-cpp\CMakeFiles /S /Q
rmdir jbinding-cpp\javah /S /Q
rmdir jbinding-cpp\Debug /S /Q
rmdir jbinding-cpp\Release /S /Q
del jbinding-java\sevenzipjbinding.jar
rmdir jbinding-java\bin-core /S /Q
rmdir Windows-x86 /S /Q
rmdir x64 /S /Q
rmdir Win32 /S /Q
rmdir Debug /S /Q
rmdir prepackage.tmp /S /Q
rmdir Testing /S /Q
