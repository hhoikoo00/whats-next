#include <emscripten.h>

#include "libbackend/csv_reader.h"
#include "libbackend/guess.h"

EMSCRIPTEN_KEEPALIVE
unsigned int get_days_in_dataset(void) {
  init_csv("total_cases.csv");
  unsigned int days_in_dataset = get_num_rows() - 1; // Ignore header row
  close_csv();
  return days_in_dataset;
}

EMSCRIPTEN_KEEPALIVE
unsigned int get_day_zero(char *country_name) {
  init_csv("total_cases.csv");
  unsigned int *column_data = get_column_as_uint(country_name);

  /* Find day zero, i.e. the first day cases are more than 100 */
  int day_zero;
  for (day_zero = 0; column_data[day_zero] <= 100; ++day_zero);

  free(column_data);
  close_csv();
  return day_zero;
}

EMSCRIPTEN_KEEPALIVE
unsigned int get_num_countries(void) {
  init_csv("total_cases.csv");
  unsigned int num_countries = get_num_columns() - 1; // Ignore date column
  close_csv();
  return num_countries;
}

EMSCRIPTEN_KEEPALIVE
char *get_start_date(void) {
  init_csv("total_cases.csv");
  char *start_date = get_row(0)[0];
  close_csv();
  return start_date;
}

EMSCRIPTEN_KEEPALIVE
char *get_all_countries(void) {
  unsigned int num_countries = get_num_countries();

  init_csv("total_cases.csv");
  char **countries = get_header();
  char *equals_separated_countries = calloc(num_countries,
                                            COLUMN_WIDTH * sizeof(char));
  for (int i = 2; i <= num_countries; ++i) {
    strcat(equals_separated_countries, "=");
    strcat(equals_separated_countries, countries[i]);
  }
  close_csv();
  free(countries[0]);
  free(countries);
  return &equals_separated_countries[1]; // Ignore starting '='
}

EMSCRIPTEN_KEEPALIVE
unsigned int *get_most_updated_cases(void) {
  /* Subtract 2 from number of rows to account for header and 0-indexing */
  init_csv("total_cases.csv");
  unsigned int *all_cases = get_row_as_uint(get_num_rows() - 2);
  close_csv();
  return &all_cases[2]; // Ignore date column and world
}

EMSCRIPTEN_KEEPALIVE
unsigned int *get_country_data(char *country_name) {
  unsigned int day_zero = get_day_zero(country_name);
  init_csv("total_cases.csv");
  unsigned int *column_data = get_column_as_uint(country_name);
  close_csv();
  return &column_data[day_zero];
}

EMSCRIPTEN_KEEPALIVE
unsigned int *get_appended_data(char *country_name, int days_to_calc) {
  /* Get cases for specified country */
  init_csv("total_cases.csv");
  int max_days = get_num_rows() - 1; // Account for header
  unsigned int *column_data = get_column_as_uint(country_name);

  /* Find day zero, i.e. the first day cases are more than 100 */
  int day_zero;
  for (day_zero = 0; column_data[day_zero] <= 100; ++day_zero);

  /* Call statistics library */
  unsigned int *dest = calloc(max_days - day_zero + days_to_calc,
                              sizeof(unsigned int));
  guess(&column_data[day_zero], max_days - day_zero, dest, days_to_calc);

  close_csv();
  return dest;
}

/* For debugging only */
void run_test(void) {
  char country[] = "China";
  unsigned int *arr = get_appended_data(country, 50);
  unsigned int *arr2 = get_country_data(country);
  unsigned int dayz = get_day_zero(country);
  unsigned int max = get_days_in_dataset();
  for (int i = 0; i < max - dayz + 50; ++i) {
    printf("day %d --- predicted is: %d, actual is: %d\n", i, arr[i], arr2[i]);
  }

  printf("day zero is: %d\n", dayz);
  printf("%s\n", get_start_date());
}
