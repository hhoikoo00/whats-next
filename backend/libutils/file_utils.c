#include "file_utils.h"

static uint32_t get_file_size(FILE *fp) {
  fseek(fp, 0, SEEK_END);
  uint32_t file_size_in_bytes = ftell(fp);
  // if somehow this overflows, file_size is more than 4GB...
  rewind(fp);
  return file_size_in_bytes;
}

bool check_file(const char file_name[]) {
  // File name checks
  if (file_name == NULL || file_name[0] == '\0') {
    return false;
  }

  // Check for read access to file, also ensures not directory
  return access(file_name, R_OK) == 0;
}

FILE *load_delay(const char file_name[], bool binary_mode){
  if (!check_file(file_name)) {
    return NULL;
  }
  FILE *fp = binary_mode ? fopen(file_name, "rb") : fopen(file_name, "r");
  if (fp == NULL) { // File failed to open at all
    return NULL;
  }
  return fp;
}

bool load_immediate(const char file_name[], void *array_dest,
                    size_t length, size_t elem_size){

  FILE *fp = load_delay(file_name, true);
  if (fp == NULL) {
    return false; // Some error in opening file
  }

  uint32_t file_size_in_bytes = get_file_size(fp);
  if (file_size_in_bytes > length * elem_size || file_size_in_bytes == 0) {
    fclose(fp);  // Not enough space to store data on array
    return false;
  }
  uint32_t num_bytes_read = fread(array_dest,
                                  1, // Read byte-by-byte
                                  file_size_in_bytes,
                                  fp);

  fclose(fp);

  // Check that we have read the file properly
  return num_bytes_read == file_size_in_bytes;
}

bool write_file(void *data, size_t length, size_t elem_size, char file_name[]) {

  // File name checks and file does not exist
  if (file_name == NULL || file_name[0] == '\0'
      || access(file_name, F_OK) == 0) {
    return false;
  }

  FILE *fp = fopen(file_name, "wb");
  uint32_t bytes_written = fwrite(data, elem_size, length, fp);

  return (fclose(fp) == 0) && (bytes_written == (elem_size * length));
}
