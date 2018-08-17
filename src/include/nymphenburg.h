/*
 *  nymphenburg.h
 *          Fundamental C definitions.
 */
#ifndef NYMPHENBURG_H
  #define NYMPHENBURG_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "common/error.h"
#include "common/memutils.h"
#include "common/netio.h"

#define true 1
#define false 0

/*
  * Pointer
  *      Variable holding address of any memory resident object.
  *
  *      XXX Pointer arithmetic is done with this, so it can't be void *
  *      under "true" ANSI compilers.
  */
 typedef char *Pointer;

 /*
 * lengthof
 *      Number of elements in an array.
 */
#define lengthof(array) (sizeof (array) / sizeof ((array)[0]))

#endif
