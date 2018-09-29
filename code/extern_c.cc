#include <stddef.h>

extern "C" size_t strlen(const char *);

#ifdef __cplusplus
extern "C" {
#endif

    int strcmp(const char*, const char*);
    char* strcat(char*, const char*);

#ifdef __cplusplus
}
#endif

// f1 is a C function;
// Its parameter is a pointer to a C function
extern "C" void f1(void(*)(int));

// FC is a pointer to a C function
extern "C" typedef void FC(int);

// f2 is a C++ function with a parameter that is a pointer to a C function
void f2(FC *);


//////////////////////////////////////////////////
// Exporting Our C++ Functions to Other Languages

// the calc function can be called from C programs
extern "C" double calc(double dparm) {
    /* ... */
}
