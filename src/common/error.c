#include <signal.h>
#include <string.h>
#include "nymphenburg.h"

void error_fatal( char * p_error )
{
  fputs(p_error, stderr);
  raise(SIGABRT);
}

void error_warning (char * p_error)
{
  perror(p_error);
}
