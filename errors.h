#include <stdio.h>
#include <stdarg.h> /* for va_list, va_args_ etc used in errors.c */


/* enum for generic safe memerory allocation functions */
typedef enum{
    sMalloc,
    sCalloc,
    sRealloc
}alloc_t;