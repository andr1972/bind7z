#ifndef __7Z_THREADS_H
#define __7Z_THREADS_H

#ifdef _WIN32
  #include "ThreadsWin.h"
#else
  #include "ThreadsP.h"
#endif

#endif //__7Z_THREADS_H
