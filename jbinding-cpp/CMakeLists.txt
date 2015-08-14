PROJECT(SevenZipJBindingCPP)

GET_PROPERTY(SEVENZIP_JBINDING_JAR  GLOBAL PROPERTY SEVENZIP_JBINDING_JAR) 

IF(NOT WIN32)
   SET(HAVE_PTHREADS 0)
   FIND_PACKAGE(Threads)
   IF(CMAKE_USE_PTHREADS_INIT)
      SET(HAVE_PTHREADS 1)
   ENDIF()
ENDIF()

IF (CMAKE_SYSTEM_NAME MATCHES "CYGWIN.*")
    SET(USE_MINGW 1)
    SET(USE_CYGWIN 1)
    MESSAGE("-- Compiling on cygwin using -mnocyg (MinGW mode)")
ELSE()
    IF(CMAKE_SYSTEM_NAME MATCHES "Windows.*")
        SET(USE_MINGW 1)
        MESSAGE("-- Compiling on windows (MinGW)")
    ELSE()
        MESSAGE("-- Compiling on posix system: ${CMAKE_SYSTEM_NAME}")
    ENDIF()
ENDIF()

IF(USE_MINGW_W64)
    INCLUDE(../cmake/FindMinGWm10-DLL.cmake)
ENDIF(USE_MINGW_W64)

SET_PROPERTY(GLOBAL PROPERTY USE_MINGW ${USE_MINGW}) 

IF(HAVE_WCHAR_H)
  OPTION(HAVE_UNICODE_WCHAR "Enable Unicode Support" YES)
  MARK_AS_ADVANCED(HAVE_UNICODE_WCHAR)
ELSE(HAVE_WCHAR_H)
  SET(HAVE_UNICODE_WCHAR 0)
ENDIF(HAVE_WCHAR_H)

SET(P7ZIP_SRC ../p7zip)

SET(JBINDING_PLATFORM_CPP_FILES)
IF(USE_MINGW)
    include_directories(PlatformMinGW/)
ELSE(USE_MINGW)
    include_directories(PlatformUnix/)
ENDIF(USE_MINGW)

include_directories(/usr/include)
include_directories(/usr/include/c++/4.3)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${JAVA_INCLUDE_PATH2})
include_directories(${P7ZIP_SRC}/CPP)
include_directories(${PROJECT_BINARY_DIR}/javah)
include_directories(.)

IF(USE_CYGWIN)
    # TODO Use properly way to set this gcc flag
    add_definitions(-mno-cygwin)
ENDIF(USE_CYGWIN)

IF(USE_MINGW)
	IF(USE_MINGW_W64)
		add_definitions(-DMINGW -mthreads -D_JNI_IMPLEMENTATION_ -D_OLEAUT32_ -D_USER32_)
	ELSE()
		add_definitions(-DMINGW -mthreads -D_JNI_IMPLEMENTATION_)
	ENDIF()
ENDIF(USE_MINGW)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE("---------- DEBUG MODE ---------------")
    add_definitions(
        -DTRACE_OBJECTS_ON
        -D_DEBUG
    )
ELSE()
    add_definitions(
        -DNDEBUG
    )
ENDIF()

add_definitions(
#    -DTRACE_ON
    -D_FILE_OFFSET_BITS=64
    -D_LARGEFILE_SOURCE
    -D_REENTRANT
    -DENV_UNIX
    -DCOMPRESS_MT
    -DCOMPRESS_BZIP2_MT
    -DCOMPRESS_MF_MT
    -DBREAK_HANDLER
    -DBENCH_MT
)

SET(P7ZIP_SOURCE_FILES
    ${P7ZIP_SRC}/C/7zBuf2.c
    ${P7ZIP_SRC}/C/7zCrc.c
    ${P7ZIP_SRC}/C/7zCrcOpt.c
    ${P7ZIP_SRC}/C/7zStream.c
    ${P7ZIP_SRC}/C/Aes.c
    ${P7ZIP_SRC}/C/Alloc.c
    ${P7ZIP_SRC}/C/Bra.c
    ${P7ZIP_SRC}/C/Bra86.c
    ${P7ZIP_SRC}/C/BraIA64.c
    ${P7ZIP_SRC}/C/BwtSort.c
    ${P7ZIP_SRC}/C/CpuArch.c
    ${P7ZIP_SRC}/C/Delta.c
    ${P7ZIP_SRC}/C/HuffEnc.c
    ${P7ZIP_SRC}/C/LzFind.c
    ${P7ZIP_SRC}/C/LzFindMt.c
    ${P7ZIP_SRC}/C/Lzma2Dec.c
    ${P7ZIP_SRC}/C/Lzma2Enc.c
    ${P7ZIP_SRC}/C/Lzma86Dec.c
    ${P7ZIP_SRC}/C/Lzma86Enc.c
    ${P7ZIP_SRC}/C/LzmaDec.c
    ${P7ZIP_SRC}/C/LzmaEnc.c
    ${P7ZIP_SRC}/C/MtCoder.c
    ${P7ZIP_SRC}/C/Ppmd7.c
    ${P7ZIP_SRC}/C/Ppmd7Dec.c
    ${P7ZIP_SRC}/C/Ppmd7Enc.c
    ${P7ZIP_SRC}/C/Ppmd8.c
    ${P7ZIP_SRC}/C/Ppmd8Dec.c
    ${P7ZIP_SRC}/C/Ppmd8Enc.c
    ${P7ZIP_SRC}/C/Sha256.c
    ${P7ZIP_SRC}/C/Sort.c
    ${P7ZIP_SRC}/C/Xz.c
    ${P7ZIP_SRC}/C/XzCrc64.c
    ${P7ZIP_SRC}/C/XzCrc64Opt.c
    ${P7ZIP_SRC}/C/XzDec.c
    ${P7ZIP_SRC}/C/XzEnc.c
    ${P7ZIP_SRC}/C/XzIn.c

    ${P7ZIP_SRC}/CPP/7zip/Archive/ApmHandler.cpp
    #${P7ZIP_SRC}/CPP/7zip/Archive/ArchiveExports.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/ArHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/ArjHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Bz2Handler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/ComHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/CpioHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/CramfsHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/DeflateProps.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/DmgHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/ElfHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/FatHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/FlvHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/GzHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/HfsHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/IhexHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/LzhHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/LzmaHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/MachoHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/MbrHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/MslzHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/MubHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/NtfsHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/PeHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/PpmdHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/RpmHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/SplitHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/SquashfsHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/SwfHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/UefiHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/VhdHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/XarHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/XzHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/ZHandler.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zCompressionMode.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zDecode.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zEncode.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zExtract.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zFolderInStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zFolderOutStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zHandlerOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zHeader.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zProperties.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zSpecStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/7z/7zUpdate.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Cab/CabBlockInStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Cab/CabHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Cab/CabHeader.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Cab/CabIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Cab/CabRegister.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Chm/ChmHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Chm/ChmIn.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/CoderMixer2.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/CoderMixer2MT.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/CoderMixer2ST.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/CrossThreadProgress.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/DummyOutStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/FindSignature.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/HandlerOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/InStreamWithCRC.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/ItemNameUtils.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/MultiStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/OutStreamWithCRC.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/OutStreamWithSha1.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Common/ParseProperties.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Iso/IsoHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Iso/IsoHeader.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Iso/IsoIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Iso/IsoRegister.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Nsis/NsisDecode.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Nsis/NsisHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Nsis/NsisIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Nsis/NsisRegister.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Rar/RarHandler.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Tar/TarHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Tar/TarHandlerOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Tar/TarHeader.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Tar/TarIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Tar/TarOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Tar/TarRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Tar/TarUpdate.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Udf/UdfHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Udf/UdfIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Udf/UdfRegister.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Wim/WimHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Wim/WimHandlerOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Wim/WimIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Wim/WimRegister.cpp

    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipAddCommon.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipHandler.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipHandlerOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipIn.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipItem.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipOut.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Archive/Zip/ZipUpdate.cpp

    ${P7ZIP_SRC}/CPP/7zip/Common/CreateCoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/CWrappers.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/FilePathAutoRename.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/FileStreams.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/FilterCoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/InBuffer.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/InOutTempBuffer.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/LimitedStreams.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/LockedStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/MemBlocks.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/MethodId.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/MethodProps.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/OffsetStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/OutBuffer.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/OutMemStream.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/ProgressMt.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/ProgressUtils.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/PropId.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/StreamBinder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/StreamObjects.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/StreamUtils.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/UniqBlocks.cpp
    ${P7ZIP_SRC}/CPP/7zip/Common/VirtThread.cpp

    ${P7ZIP_SRC}/CPP/7zip/UI/Common/LoadCodecs.cpp
    ${P7ZIP_SRC}/CPP/7zip/UI/Common/PropIDUtils.cpp

    ${P7ZIP_SRC}/CPP/7zip/Compress/ArjDecoder1.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ArjDecoder2.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Bcj2Coder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Bcj2Register.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BcjCoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BcjRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BitlDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BranchCoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BranchMisc.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BranchRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ByteSwap.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BZip2Crc.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BZip2Decoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BZip2Encoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/BZip2Register.cpp
    #${P7ZIP_SRC}/CPP/7zip/Compress/CodecExports.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/CopyCoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/CopyRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Deflate64Register.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/DeflateDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/DeflateEncoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/DeflateRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/DeltaFilter.cpp
    #${P7ZIP_SRC}/CPP/7zip/Compress/DllExports2Compress.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ImplodeDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ImplodeHuffmanDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/LzhDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Lzma2Decoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Lzma2Encoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Lzma2Register.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/LzmaDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/LzmaEncoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/LzmaRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/LzOutWindow.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Lzx86Converter.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/LzxDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/PpmdDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/PpmdEncoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/PpmdRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/PpmdZip.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/QuantumDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Rar1Decoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Rar2Decoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Rar3Decoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/Rar3Vm.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/RarCodecsRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ShrinkDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ZDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ZlibDecoder.cpp
    ${P7ZIP_SRC}/CPP/7zip/Compress/ZlibEncoder.cpp

    ${P7ZIP_SRC}/CPP/7zip/Crypto/7zAes.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/7zAesRegister.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/HmacSha1.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/MyAes.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/MyAesReg.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/Pbkdf2HmacSha1.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/RandGen.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/Rar20Crypto.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/RarAes.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/Sha1.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/Sha1Reg.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/WzAes.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/ZipCrypto.cpp
    ${P7ZIP_SRC}/CPP/7zip/Crypto/ZipStrong.cpp

    ${P7ZIP_SRC}/CPP/Common/CommandLineParser.cpp
    ${P7ZIP_SRC}/CPP/Common/CRC.cpp
    ${P7ZIP_SRC}/CPP/Common/CrcReg.cpp
    ${P7ZIP_SRC}/CPP/Common/DynLimBuf.cpp
    ${P7ZIP_SRC}/CPP/Common/IntToString.cpp
    ${P7ZIP_SRC}/CPP/Common/Lang.cpp
    ${P7ZIP_SRC}/CPP/Common/ListFileUtils.cpp
    ${P7ZIP_SRC}/CPP/Common/MyMap.cpp
    ${P7ZIP_SRC}/CPP/Common/MyString.cpp
    ${P7ZIP_SRC}/CPP/Common/MyVector.cpp
    ${P7ZIP_SRC}/CPP/Common/MyXml.cpp
    ${P7ZIP_SRC}/CPP/Common/NewHandler.cpp
    ${P7ZIP_SRC}/CPP/Common/Sha256Reg.cpp
    #${P7ZIP_SRC}/CPP/Common/StdInStream.cpp
    #${P7ZIP_SRC}/CPP/Common/StdOutStream.cpp
    ${P7ZIP_SRC}/CPP/Common/StringToInt.cpp
    ${P7ZIP_SRC}/CPP/Common/TextConfig.cpp
    ${P7ZIP_SRC}/CPP/Common/UTFConvert.cpp
    ${P7ZIP_SRC}/CPP/Common/Wildcard.cpp
    ${P7ZIP_SRC}/CPP/Common/XzCrc64Reg.cpp

    #${P7ZIP_SRC}/CPP/myWindows/myAddExeFlag.cpp
    #${P7ZIP_SRC}/CPP/myWindows/mySplitCommandLine.cpp
    #${P7ZIP_SRC}/CPP/myWindows/test_emul.cpp
    #${P7ZIP_SRC}/CPP/myWindows/wine_date_and_time.cpp
    #${P7ZIP_SRC}/CPP/myWindows/wine_GetXXXDefaultLangID.cpp

    #${P7ZIP_SRC}/CPP/Windows/Clipboard.cpp
    ${P7ZIP_SRC}/CPP/Windows/COM.cpp
    #${P7ZIP_SRC}/CPP/Windows/DLL.cpp
    ${P7ZIP_SRC}/CPP/Windows/ErrorMsg.cpp
    ${P7ZIP_SRC}/CPP/Windows/PropVariant.cpp
    ${P7ZIP_SRC}/CPP/Windows/PropVariantConv.cpp
    ${P7ZIP_SRC}/CPP/Windows/PropVariantUtils.cpp
    ${P7ZIP_SRC}/CPP/Windows/TimeUtils.cpp
    #${P7ZIP_SRC}/CPP/Windows/Window.cpp

    #${P7ZIP_SRC}/CPP/Windows/Control/Controls.cpp
    #${P7ZIP_SRC}/CPP/Windows/Control/Dialog.cpp
    #${P7ZIP_SRC}/CPP/Windows/Control/Window2.cpp
)

IF(USE_MINGW)
SET(P7ZIP_SOURCE_FILES_DEP
    ${P7ZIP_SRC}/C/ThreadsWin.c
    ${P7ZIP_SRC}/CPP/Common/StringConvertWin.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileDirWin.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileFindWin.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileIOWin.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileNameWin.cpp
    ${P7ZIP_SRC}/CPP/Windows/RegistryWin.cpp
    ${P7ZIP_SRC}/CPP/Windows/SystemWin.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileLink.cpp
)
ELSE(USE_MINGW)
SET(P7ZIP_SOURCE_FILES_DEP
    ${P7ZIP_SRC}/C/Threads.c
    ${P7ZIP_SRC}/CPP/Common/MyWindows.cpp
    ${P7ZIP_SRC}/CPP/Common/StringConvert.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileDir.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileFind.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileIO.cpp
    ${P7ZIP_SRC}/CPP/Windows/FileName.cpp
    #${P7ZIP_SRC}/CPP/Windows/Registry.cpp #wxWidgets
    ${P7ZIP_SRC}/CPP/Windows/Synchronization.cpp
    ${P7ZIP_SRC}/CPP/Windows/System.cpp
)
ENDIF(USE_MINGW)

SET(JBINDING_CPP_FILES
#    Debug.cpp
#    idd_def.cpp
    JNITools.cpp
    JNICallState.cpp
    SevenZipException.cpp
    SevenZipJBinding.cpp
    UniversalArchiveOpenCallback.cpp
    JavaToCPP/JavaToCPPInArchiveImpl.cpp
    JavaToCPP/JavaToCPPSevenZip.cpp
    CPPToJava/CPPToJavaArchiveExtractCallback.cpp
    CPPToJava/CPPToJavaArchiveOpenCallback.cpp
    CPPToJava/CPPToJavaArchiveOpenVolumeCallback.cpp
    CPPToJava/CPPToJavaCryptoGetTextPassword.cpp
    CPPToJava/CPPToJavaInStream.cpp
    CPPToJava/CPPToJavaProgress.cpp
    CPPToJava/CPPToJavaSequentialInStream.cpp
    CPPToJava/CPPToJavaSequentialOutStream.cpp
)

SET(JBINDING_JAVAH_H_FILES
    ${PROJECT_SOURCE_DIR}/JavaToCPP/Java/net_sf_sevenzipjbinding_impl_InArchiveImpl.h
    ${PROJECT_SOURCE_DIR}/JavaToCPP/Java/net_sf_sevenzipjbinding_SevenZip.h
)

#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")

SET(CMAKE_VERBOSE_MAKEFILE OFF)
IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

#ADD_EXECUTABLE(Client7z Client7z.cpp ${P7ZIP_SOURCE_FILES})
#TARGET_LINK_LIBRARIES(Client7z dl)

#<exec executable="${javah}">                                                                                 
#                        <arg value="-jni" />                                                                                 
#                        <arg value="-d" />                                                                                   
#                        <arg path="${workspace}/${sevenzip.javaheaders}" />                                                  
#                        <arg value="-force" />                                                                               
#                        <arg value="-classpath" />                                                                           
#                        <arg path="${basedir}/bin" />                                                                        
#                        <arg value="net.sf.sevenzip.SevenZip" />                                                             
#                        <arg value="net.sf.sevenzip.impl.InArchiveImpl" />                                                   
#                </exec>
SET(JAVAH_OUTPUT_DIR ${PROJECT_BINARY_DIR}/javah)
FILE(MAKE_DIRECTORY ${JAVAH_OUTPUT_DIR})
SET(JAVAH_JAVA_CLASS_LIST
    net.sf.sevenzipjbinding.SevenZip
    net.sf.sevenzipjbinding.impl.InArchiveImpl
)


add_custom_command(OUTPUT ${JBINDING_JAVAH_H_FILES}
                   COMMAND ${JAVA_HEADER_COMPILE} -jni -d . -classpath ${SEVENZIP_JBINDING_JAR} ${JAVAH_JAVA_CLASS_LIST}  
                   DEPENDS ${SEVENZIP_JBINDING_JAR}
                   WORKING_DIRECTORY ${JAVAH_OUTPUT_DIR}
                   COMMENT "javah: generating h-files from java classes ${JAVAH_JAVA_CLASS_LIST}"
)

add_library(7-Zip-JBinding
            SHARED
            ${JBINDING_CPP_FILES}
            ${JBINDING_PLATFORM_CPP_FILES}
            ${P7ZIP_SOURCE_FILES}
            ${P7ZIP_SOURCE_FILES_DEP}
            ${JBINDING_JAVAH_H_FILES})

#GET_TARGET_PROPERTY(SEVENZIP_JBINDING_LIB 7-Zip-JBinding LOCATION)
#SET_PROPERTY(GLOBAL PROPERTY SEVENZIP_JBINDING_LIB ${SEVENZIP_JBINDING_LIB}) 

IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    # TODO Get status to work on Linux64
    IF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "i686") 
        SET_TARGET_PROPERTIES(7-Zip-JBinding PROPERTIES LINK_FLAGS -static)
    ENDIF()
ENDIF()

IF(USE_CYGWIN)
    TARGET_LINK_LIBRARIES(7-Zip-JBinding oleaut32)
ELSE(USE_CYGWIN)
    IF(USE_MINGW)
#       TARGET_LINK_LIBRARIES(7-Zip-JBinding c)
    ELSE(USE_MINGW)
       TARGET_LINK_LIBRARIES(7-Zip-JBinding dl c)
    ENDIF(USE_MINGW)
ENDIF(USE_CYGWIN)

IF(USE_CYGWIN)
    SET_TARGET_PROPERTIES(7-Zip-JBinding PROPERTIES LINK_FLAGS "-mno-cygwin -Wl,--add-stdcall-alias -mthreads") # -lpthread -lstdc++ -lws2_32") #-Wl,--add-stdcall-alias
ELSE(USE_CYGWIN)
    IF(USE_MINGW)
        SET_TARGET_PROPERTIES(7-Zip-JBinding PROPERTIES LINK_FLAGS "-Wall -Wl,--kill-at -static -mthreads")
        
        # Remove default libraries 
        #SET(CMAKE_CXX_STANDARD_LIBRARIES "-g") #-luser32 -loleaut32 -lole32 -lshell32 -lgdi32 -lwinspool -luuid -lcomdlg32 -ladvapi32 -lkernel32 
        #SET(CMAKE_C_STANDARD_LIBRARIES "-g") #-luser32 -loleaut32 -lole32 -lshell32 -lgdi32 -lwinspool -luuid -lcomdlg32 -ladvapi32 -lkernel32 
    ENDIF(USE_MINGW)
ENDIF(USE_CYGWIN)

#TARGET_LINK_LIBRARIES(7-Zip-JBinding duma)
#FILE(WRITE "link.sh" "${CMAKE_CXX_CREATE_SHARED_LIBRARY}")
#IF(CMAKE_COMPILER_IS_GNUCXX)
#  IF(CMAKE_COMPILER_IS_GNUCC)
#    SET(CMAKE_CXX_CREATE_SHARED_LIBRARY
#        "gcc <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> <CMAKE_SHARED_LIBRARY_SONAME_CXX_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
#  ENDIF()
#ENDIF()
