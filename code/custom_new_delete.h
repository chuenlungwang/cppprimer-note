#ifndef __CUSTOM_NEW_DELETE_H__
#define __CUSTOM_NEW_DELETE_H__

#include <stdlib.h>
#include <stdexcept>

void *operator new(size_t size)
{
    if (void *mem = malloc(size))
        return mem;
    else
        throw std::bad_alloc();
}

void operator delete(void *mem) noexcept(true)
{
    free(mem);
}

#endif
