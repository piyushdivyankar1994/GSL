#ifndef GSL_SORT_FLOAT_H
#define GSL_SORT_FLOAT_H

#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_permutation.h>

void gsl_sort_float (float * data, size_t stride, size_t n);
int gsl_sort_float_index (size_t * p, const float * data, size_t stride, size_t n);

#endif /* GSL_SORT_FLOAT_H */
