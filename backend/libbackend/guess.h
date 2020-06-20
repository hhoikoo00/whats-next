#ifndef GUESS_H
#define GUESS_H

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "stdlib.h"

void guess(const unsigned int *input, int input_size,
           unsigned int *dest, int days_to_calc);

void test(const unsigned int *input, int input_size,
          unsigned int *dest, int days_to_calc);

#endif //GUESS_H
