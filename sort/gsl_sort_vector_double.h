#ifndef GSL_SORT_VECTOR_DOUBLE_H
#define GSL_SORT_VECTOR_DOUBLE_H

#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector_double.h>

void gsl_sort_vector (gsl_vector * v);
void gsl_sort_vector_index (gsl_permutation * p, const gsl_vector * v);

#endif /* GSL_SORT_VECTOR_DOUBLE_H */