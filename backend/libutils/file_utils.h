#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/* load_immediate only reads in binary files. If destination size is larger
 * than is required, load immediate only uses what is required.
 * max_file_size to be provided in bytes
 * load_delay returns a NULL type if attempt to open is unsuccessful
 */
bool check_file(const char file_name[]);
bool load_immediate(const char file_name[], void *array_dest,
                size_t length, size_t elem_size);
FILE *load_delay(const char file_name[], bool binary_mode);
bool write_file(void *data, size_t length, size_t elem_size, char file_name[]);

#endif //FILE_UTILS_H
