#ifndef GSL_SORT_VECTOR_USHORT_H
#define GSL_SORT_VECTOR_USHORT_H

#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector_ushort.h>

void gsl_sort_vector_ushort (gsl_vector_ushort * v);
void gsl_sort_vector_ushort_index (gsl_permutation * p, const gsl_vector_ushort * v);

#endif /* GSL_SORT_VECTOR_USHORT_H */