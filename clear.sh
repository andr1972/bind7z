#!/bin/bash
rm Makefile
rm SevenZipJBinding.cbp
rm SevenZipJBinding.layout
rm *.tmp
rm DartConfiguration.tcl
rm CMakeCache.txt
rm sevenzipjbinding-platforms.properties
#http://askubuntu.com/questions/256521/removing-files-with-a-certain-extension-except-one-file-from-terminal
for X in *.cmake; do
    if [ "$X" != "CTestConfig.cmake" ]; then
        rm "$X"
    fi
done
rm -r CMakeFiles
rm -r javac-test
rm -r jbinding-cpp/CMakeFiles 
rm jbinding-cpp/cmake_install.cmake
rm jbinding-cpp/SevenZipJBindingCPP.cbp
rm jbinding-cpp/Makefile
rm -r jbinding-cpp/CMakeFiles
rm -r jbinding-cpp/javah
rm jbinding-java/sevenzipjbinding.jar
rm -r jbinding-java/bin-core
rm -r Linux-amd64 
rm -r p7zip/*.o
find p7zip -name '*.o' -delete
rm jbinding-cpp/lib7-Zip-JBinding.so