#include <float.h>
#include <stdbool.h>

#include "guess.h"

#define R0MIN 100
#define R0MAX 400
#define DAYS_IN_WEEK 7
#define START_OFFSET 10
#define GRADIENT_FACTOR 10
#define B_SCALE_FACTOR 2

static double calculate(double a, double b, double c, int t) {
  return round(c / (1 + a * exp(-b * t)));
}

void guess(const unsigned int *input, int input_size,
           unsigned int *dest, int days_to_calc) {

  /* Calculate gradients of curve */
  double max_gradient = 0;
  double curr_gradient = 0;
  for (int days = START_OFFSET; days < input_size; ++days) {
    curr_gradient = ((double) input[days] - input[days - 1]);
    max_gradient = max_gradient < curr_gradient ? curr_gradient : max_gradient;
  }

  /* Get gradient of last 7 days */
  double avg_gradient = 0;
  for (int days = input_size - 1; days >= input_size - DAYS_IN_WEEK; --days) {
    avg_gradient += ((double) input[days] - input[days - 1]);
  }

  /* Scale data appropriately, based on some testing */
  avg_gradient = avg_gradient / DAYS_IN_WEEK;
  max_gradient = max_gradient / (double) input[input_size - 1];
  avg_gradient = avg_gradient / (double) input[input_size - 1];

  /* Guess a, b and c appropriately  */
  double c = input[input_size - 1] * (1 + avg_gradient * 100);
  double a = c / input[0] - 1;
  double b = 0;

  /* Iteratively determine the optimal b */
  for (int j = 1; j < input_size; ++j) {
    b += ((double) input[j] - input[j - 1]) / (double) input[j];
  }

  double least = DBL_MAX;
  double r0, j_prime, b_prime, sum;
  for (int k = R0MIN; k < R0MAX; k++) {
    sum = 0;
    j_prime = (double) k / 100;
    b_prime = b / input_size * j_prime;

    /* Compare our intermediate result to actual result */
    for (int i = 0; i < input_size; i++) {
      sum += ((double) input[i] - calculate(a, b_prime, c, i)) /
             (double) input[i];
    }

    /* Check if calculated values are the best so far */
    sum = fabs(sum);
    if (sum / input_size < least) {
      least = sum / input_size;
      r0 = j_prime;
    }
  }

  /* Scale b appropriately depending on the ratio of avg to max gradient */
  bool is_bigger = avg_gradient * GRADIENT_FACTOR > max_gradient;
  b = (b / input_size) * r0;
  b = is_bigger ? b : b * B_SCALE_FACTOR;

  /* Calculate prediction using our values of a, b and c */
  for (int i = 0; i < days_to_calc + input_size; i++) {
    dest[i] = (unsigned int) calculate(a, b, c, i);
  }
}

void test(const unsigned int *input, int input_size,
          unsigned int *dest, int days_to_calc) {
  guess(input, input_size, dest, days_to_calc);
  double sum = 0;
  for (int i = 0; i < input_size; i++) {
    sum +=  fabs((double) input[i] - dest[i]) / (double) input[i];
  }
  printf("Average difference is %f\n", sum / (double) input_size);
}
