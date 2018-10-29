
#ifndef COMMON_H
  #define COMMON_H

#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#include "common/error.h"
#include "common/memutils.h"
#include "common/netio.h"
#include "common/list.h"
#include "common/signalsHandler.h"
#include "common/timeHelper.h"

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#define true 1
#define false 0

 /*
 * lengthof
 *      Number of elements in an array.
 */
#define lengthof(array) (sizeof(array) / sizeof((array)[0]))

#endif
