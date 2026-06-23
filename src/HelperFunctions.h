#pragma once

#include <string>
#include <stdexcept>

#include <k.h>


//////////////////////
// WINDOWS BINDINGS //
//////////////////////

#ifdef _WIN32
#define EXP __declspec(dllexport)
#else
#define EXP __attribute__((visibility("default")))
#endif // _WIN32


/////////////////
// KDB STRINGS //
/////////////////

inline bool IsKdbString(K str)
{
  return str != NULL && (str->t == -KS || str->t == KC);
}

inline const std::string GetKdbString(K str)
{
  return str->t == -KS ? str->s : std::string((S)kG(str), str->n);
}


////////////////////////
// EXCEPTION HANDLING //
////////////////////////

#define KDB_EXCEPTION_TRY \
  static char error_msg[1024]; \
  *error_msg = '\0'; \
  try {

#define KDB_EXCEPTION_CATCH \
  } catch (std::exception& e) {  \
    strncpy(error_msg, e.what(), sizeof(error_msg));  \
    error_msg[sizeof(error_msg) - 1] = '\0';  \
    return krr(error_msg);  \
  }


///////////////////
// GENERIC NULLS //
///////////////////

inline K Identity()
{
  K id = ka(101);
  id->g = 0;
  return id;
}
