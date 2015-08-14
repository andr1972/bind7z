// Windows/FileIO.h

#ifndef __WINDOWS_FILEIO_H
#define __WINDOWS_FILEIO_H

#ifdef _WIN32
  #include "FileIOWin.h"
#else
  #include "FileIOP.h"
#endif

#endif