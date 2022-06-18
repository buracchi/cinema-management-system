#pragma once

#if defined(_WIN32)
#  if defined(EXPORTING_PAYMENT_SERVICE_PLUGIN)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

#include <iostream>

DECLSPEC bool foo();
