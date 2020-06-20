#include "file_iterator.h"

static FILE *fp;
static char *buffer;
static uint32_t internal_size;

bool init_file_reader(const char file_name[], uint32_t buffer_size) {
  internal_size = buffer_size;
  buffer = calloc(1, internal_size);

  // Open file not in binary mode
  fp = load_delay(file_name, false);

  if (fp == NULL) {
    free(buffer);
  }

  return fp != NULL;
}

bool reader_has_next(void) {
  if (fp == NULL) {
    return false;
  }

  char internal_buffer[internal_size];
  memset(internal_buffer, 0, internal_size * sizeof(char)); // 0-initialise array
  
  // Check from current file pointer to next newline char
  bool test = fgets(internal_buffer, internal_size, fp) != NULL;
  
  // Reset pointer, -1 to account for null char at end of string
  fseek(fp, -strcspn(internal_buffer, "\n") - 1, SEEK_CUR);

  return test;
}

char *reader_get_next(void) {
  if (fp == NULL || fgets(buffer, internal_size, fp) == NULL) {
    return NULL;
  }
  
  // if it was only a newline char get next line or return NULL if eof
  if (strcmp(buffer, "\n") == 0) {
    if (reader_has_next()) {
      return reader_get_next();
    } else {
      return NULL;
    } 
  }

  buffer[strlen(buffer) - 1] = '\0'; // Replace newline char with null char
  
  return buffer;
}

bool reader_close_file(void) {
  free(buffer);
  return fclose(fp);
}
