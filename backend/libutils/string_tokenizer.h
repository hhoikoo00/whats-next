#ifndef STRING_TOKENIZER_H
#define STRING_TOKENIZER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* WARNING: input must NOT be string literal
 * Function writes to num_token + 1 entries in dest, to account for
 * rest of string left. If there are no strings left after tokenizing,
 * then only num_token entries are written to
 *
 * Returns false if error occurred
 */

char *strip_whitespace(char *input);
bool tokenize(char delimiter[], char input[], size_t buffer_length,
              char dest[][buffer_length], size_t num_token);

#endif //STRING_TOKENIZER_H
