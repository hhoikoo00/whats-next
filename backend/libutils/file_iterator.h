#ifndef FILE_ITERATOR_H
#define FILE_ITERATOR_H

#include <stdlib.h>
#include <string.h>

#include "file_utils.h"

/* Performs like an iterator for reading string files */
bool init_file_reader(const char file_name[], uint32_t buffer_size);
bool reader_has_next(void);
char *reader_get_next(void);
bool reader_close_file(void);

#endif //FILE_ITERATOR_H
