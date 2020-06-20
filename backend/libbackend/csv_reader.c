#include "csv_reader.h"

static int num_columns = -1;
static int num_rows = -1;
static char **header;
static char ***raw_data;

/* Data processing helper functions */
static char *intersperse(const char *input) {
  char *dest = calloc(num_columns, COLUMN_WIDTH * sizeof(char));
  ERROR_FAIL(dest, NULL, "Calloc failed while interspersing");

  /* Deal with starting comma */
  int j = GET_FIRST_CHAR(input) == ',' ? 1 : 0;
  dest[0] = '0';

  for (int i = 0; i < (int) strlen(input); i++) {
    if (input[i] == ',' && input[i+1] == ',') {
      dest[i + j] = input[i];
      j++;
      dest[i + j] = '0';
    } else {
      dest[i + j] = input[i];
    }
  }

  /* Deal with trailing comma */
  if (GET_LAST_CHAR(dest) == ',') {
    dest[strlen(dest)] = '0';
  }

  return dest;
}

static char **parse_line(char *line) {
  char char_dest[num_columns][COLUMN_WIDTH];
  memset(char_dest, 0, num_columns * COLUMN_WIDTH * sizeof(char));
  char **row_spine = calloc(num_columns, sizeof(char_dest));
  ERROR_FAIL(row_spine, row_spine, "Calloc failed while parsing line");

  /* Intersperse in consecutive commas with 0s */
  char *interspersed = intersperse(line);

  /* Actual parsing of csv */
  tokenize(",", interspersed, COLUMN_WIDTH, char_dest, 0);

  for (int i = 0; i < num_columns; ++i) {
    row_spine[i] = calloc(COLUMN_WIDTH, sizeof(char));
    memcpy(row_spine[i], char_dest[i], COLUMN_WIDTH * sizeof(char));
  }

  free(interspersed);
  return row_spine;
}

static char ***parse_data(int line_count) {
  char *line;
  char ***spine = calloc(line_count, sizeof(char *));
  ERROR_FAIL(spine, NULL, "Calloc failed while parsing data");

  for (int i = 0; reader_has_next() && i < line_count; ++i) {
    line = reader_get_next();
    char **row_spine = parse_line(line);
    spine[i] = row_spine;
  }

  return spine;
}

/* CSV helper functions */
static int calc_num_rows(char csv_file_name[]) {
  bool open_success = init_file_reader(csv_file_name, num_columns * COLUMN_WIDTH);
  ERROR_FAIL(open_success, NULL, "Failed to open file");
  int i;
  for (i = 0; reader_has_next(); ++i) {
    reader_get_next();
  }
  reader_close_file();
  return i;
}

static int calc_num_columns(char csv_file_name[]) {
  bool open_success = init_file_reader(csv_file_name, MAX_NUM_COLUMNS * COLUMN_WIDTH);
  ERROR_FAIL(open_success, NULL, "Failed to open file");
  char *first_line = reader_get_next();
  char header_temp[MAX_NUM_COLUMNS][COLUMN_WIDTH];
  memset(header_temp, 0, sizeof(header_temp));
  tokenize(",", first_line, COLUMN_WIDTH, header_temp, 0);

  int i;
  for (i = 0; header_temp[i][0] != '\0'; ++i);

  reader_close_file();
  return i;
}

int get_column_index(char column_name[]) {
  int c_index;
  for (c_index = 0;
       c_index < num_columns;
       c_index++) {
    if (strcmp(column_name, header[c_index]) == 0) {
      return c_index;
    }
  }
  ERROR_FAIL(NULL, NULL, "Column name not found in header");
}

static bool is_initialised(void) {
  return num_columns != -1 && num_rows != -1;
}

/* CSV file IO */
bool init_csv(char csv_file_name[]) {
  /* Check if initialised already */
  if (is_initialised()) {
    printf("CSV already initialised!");
    return false;
  }

  /* Columns MUST be called first due to how
   * memory needs to be allocated for rows
   */
  num_columns = calc_num_columns(csv_file_name);
  num_rows = calc_num_rows(csv_file_name);

  bool open_success = init_file_reader(csv_file_name, num_columns * COLUMN_WIDTH);
  ERROR_FAIL(open_success, NULL, "Failed to open file");

  /* Handle header of csv file */
  char *first_line = reader_get_next();
  char header_temp[num_columns][COLUMN_WIDTH];
  memset(header_temp, 0, sizeof(header_temp));

  tokenize(",", first_line, COLUMN_WIDTH, header_temp, 0);
  header = calloc(num_columns, sizeof(char *));
  header[0] = calloc(num_columns, sizeof(char) * COLUMN_WIDTH);
  ERROR_FAIL(header, NULL, "Calloc failed while initialising csv reader");
  memcpy(header[0], header_temp[0], COLUMN_WIDTH);

  for (int i = 1; i < num_columns; ++i) {
    header[i] = header[i - 1] + COLUMN_WIDTH;
    memcpy(header[i], header_temp[i], COLUMN_WIDTH);
  }

  /* Handle reading of values with row containing data
   * for all countries for one specific date
   * parse_data continues reading from file using reader_get_next()
   */
  char ***parsed_data = parse_data(num_rows);
  raw_data = parsed_data;

  return true;
}

bool close_csv(void) {
  CHECK_INIT();

  /* Free all data and clean up*/
  for (int i = 0; i < num_rows - 1; ++i) {
    for (int j = 0; j < num_columns; ++j) {
      free(raw_data[i][j]);
    }
    free(raw_data[i]);
  }
  free(raw_data);
  num_columns = -1;
  num_rows = -1;
  return reader_close_file();
}

/* Publicly accessible methods */
char **get_column(char column_name[]) {
  CHECK_INIT();

  /* Find column index in header */
  int column = get_column_index(column_name);

  /* Data for a single column is stored top-down, so access it accordingly */
  char **res = calloc(num_rows, sizeof(char *));
  res[0] = calloc(num_rows, sizeof(char) * (COLUMN_WIDTH + 1));
  ERROR_FAIL(res, NULL, "Calloc failed while getting column");
  ERROR_FAIL(res[0], res, "Calloc failed while getting column");
  memcpy(res[0], *(raw_data[0] + column), COLUMN_WIDTH + 1);

  for (int i = 1; i < num_rows - 1; ++i) {
    res[i] = res[i - 1] + COLUMN_WIDTH;
    memcpy(res[i], *(raw_data[i] + column), COLUMN_WIDTH);
  }

  return res;
}

char **get_row(int row_number) {
  CHECK_INIT();
  char **res = calloc(num_columns, sizeof(char *));
  res[0] = calloc(num_columns, sizeof(char) * COLUMN_WIDTH);
  ERROR_FAIL(res, NULL, "Calloc failed while getting column");
  ERROR_FAIL(res[0], res, "Calloc failed while getting column");
  memcpy(res[0], raw_data[row_number][0], COLUMN_WIDTH);

  for (int i = 1; i < num_columns; ++i) {
    res[i] = res[i - 1] + COLUMN_WIDTH;
    memcpy(res[i], raw_data[row_number][i], COLUMN_WIDTH);
  }

  return res;
}

char **get_header(void) {
  CHECK_INIT();
  char **res = calloc(num_columns, sizeof(char *));
  res[0] = calloc(num_columns, sizeof(char) * COLUMN_WIDTH);
  memcpy(res[0], header[0], COLUMN_WIDTH);

  for (int i = 1; i < num_columns; ++i) {
    res[i] = res[i - 1] + COLUMN_WIDTH;
    memcpy(res[i], header[i], COLUMN_WIDTH);
  }

  return res;
}

unsigned int *get_column_as_uint(char column_name[]) {
  /* Get data as strings */
  char **string_columns = get_column(column_name);
  unsigned int *res = calloc(num_rows, sizeof(unsigned int *));

  /* Convert to ints */
  for (int i = 0; i < num_rows - 1; ++i) {
    res[i] = strtoul(string_columns[i], NULL, 0);
  }

  free(string_columns[0]);
  free(string_columns);
  return res;
}

unsigned int *get_row_as_uint(int row_number) {
  /* Get data as strings */
  char **string_rows = get_row(row_number);
  unsigned int *res = calloc(num_columns, sizeof(unsigned int *));

  /* Convert to ints */
  for (int i = 0; i < num_columns; ++i) {
    res[i] = strtoul(string_rows[i], NULL, 0);
  }

  free(string_rows[0]);
  free(string_rows);
  return res;
}

int get_num_rows(void) {
  CHECK_INIT();
  return num_rows;
}

int get_num_columns(void) {
  CHECK_INIT();
  return num_columns;
}
