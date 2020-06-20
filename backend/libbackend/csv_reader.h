#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdlib.h>
#include <string.h>

#include "../libutils/file_iterator.h"
#include "../libutils/string_tokenizer.h"

#define MAX_NUM_COLUMNS 250
#define COLUMN_WIDTH 50
#define GET_LAST_CHAR(string) (string[strlen(string) - 1])
#define GET_FIRST_CHAR(string) (string[0])

#define ERROR_FAIL(ptr, ptr2, message)            \
  do { if (!ptr) {                                \
         free(ptr2);                              \
         fprintf(stderr, "%s\n", message);        \
         return 0;                                \
     }                                            \
  } while (0)

#define CHECK_INIT()                              \
  do { if (!is_initialised()) {                   \
         printf("CSV not initialised!\n");        \
         return 0;                                \
     } \
  } while (0)

/* CSV file operations */
bool init_csv(char csv_file_name[]);
bool close_csv(void);

/* Data reading operations */
char **get_column(char column_name[]);
char **get_row(int row_number);
char **get_header(void);
unsigned int *get_column_as_uint(char column_name[]);
unsigned int *get_row_as_uint(int row_number);

/* Utility functions */
int get_num_rows(void);
int get_num_columns(void);
int get_column_index(char column_name[]);

#endif //CSV_READER_H

