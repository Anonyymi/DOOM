/*
 * This file substitutes lprintf.h from the prboom source.
 * Written by Pedro Gimeno, donated to the public domain.
 */

#define STRINGIFY(x) #x
#define lprintf(x, ...) printf(STRINGIFY(x) ": " __VA_ARGS__)
