#include "string_tokenizer.h"

char *strip_whitespace(char *input) {
  // Strip leading whitespaces
  while (isspace(*input)) {
    input = input + 1;
  }

  // Short circuiting for all-whitespace inputs
  if (strlen(input) == 0) {
    return input;
  }

  // Strip trailing whitespaces
  unsigned long end = strlen(input);
  while (isspace(input[end - 1])) {
    end -= 1;
  }
  input[end] = '\0';

  return input;
}

bool tokenize(char delimiter[], char input[], size_t buffer_length,
              char dest[][buffer_length], size_t num_token) {

  // Clean up input and parse special case where num_token = 0
  input = strip_whitespace(input);
  num_token = num_token == 0 ? SIZE_MAX : num_token;

  // Deal with empty strings or all-whitespace strings
  if (delimiter[0] == '\0' || input[0] == '\0') {
    strcpy(dest[0], input);
    return true;
  }

  // Tokenize first item
  char *token = strtok(input, delimiter);
  unsigned int i;

  // Tokenize rest of string
  for (i = 0; token != NULL && i < num_token; ++i) {
    if (buffer_length < strlen(token)) {
      return false;
    }
    strcpy(dest[i], token);
    memmove(dest[i], strip_whitespace(dest[i]), strlen(dest[i]));
    token = strtok(NULL, delimiter);
  }

  // Handling of rest of string
  if (num_token != SIZE_MAX && token != NULL) {
    char rest[buffer_length];
    strcpy(rest, token);
    strcat(rest, delimiter);

    char *temp = strtok(NULL, "");
    if (temp != NULL) {
      strcat(rest, temp);
    }
    if (buffer_length < strlen(rest)) {
      return false;
    }
    strcpy(dest[i], strip_whitespace(rest));
  }

  return true;
}
