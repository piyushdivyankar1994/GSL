#include <config.h>

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <gsl_math.h>
#include <gsl_errno.h>
#include <gsl_multiroots.h>
#include <gsl_linalg.h>

typedef struct
  {
    gsl_matrix * lu;
    gsl_vector_int * permutation;
  }
hybrid_state_t;

int hybrid_init (void * vstate, gsl_multiroot_function * function, gsl_vector * x, gsl_vector * f, gsl_vector * dx);
int hybrid_iterate (void * vstate, gsl_multiroot_function * function, gsl_vector * x, gsl_vector * f, gsl_vector * dx);
void hybrid_free (void * vstate);

int
hybrid_init (void * vstate, gsl_multiroot_function * function, gsl_vector * x, gsl_vector * f, gsl_vector * dx)
{
  hybrid_state_t * state = (hybrid_state_t *) vstate;
  size_t i, n = function->n ;
  gsl_vector_int * p;
  gsl_matrix * m;

  m = gsl_matrix_calloc (n,n);
  
  if (m == 0) 
    {
      GSL_ERROR_RETURN ("failed to allocate space for lu", GSL_ENOMEM, 0);
    }

  state->lu = m ;

  p = gsl_vector_int_calloc (n);

  if (p == 0)
    {
      gsl_matrix_free(m);

      GSL_ERROR_RETURN ("failed to allocate space for permutation", GSL_ENOMEM, 0);
    }

  state->permutation = p ;

  GSL_MULTIROOT_FN_EVAL_F (function, x, f);

  for (i = 0; i < n; i++)
    {
      gsl_vector_set (dx, i, 0.0);
    }

  return GSL_SUCCESS;
}

int
hybrid_iterate (void * vstate, gsl_multiroot_function * function, gsl_vector * x, gsl_vector * f, gsl_vector * dx)
{
  hybrid_state_t * state = (hybrid_state_t *) vstate;
  
  int signum ;

  size_t i;

  size_t n = function->n ;

  /*  gsl_matrix_copy (state->lu, J);

  gsl_la_decomp_LU_impl (state->lu, state->permutation, &signum);

  gsl_la_solve_LU_impl (state->lu, state->permutation, f, dx); */
      
  for (i = 0; i < n; i++)
    {
      double e = gsl_vector_get (dx, i);
      double y = gsl_vector_get (x, i);
      gsl_vector_set (dx, i, -e);
      gsl_vector_set (x, i, y - e);
    }

  GSL_MULTIROOT_FN_EVAL_F (function, x, f);

  return GSL_SUCCESS;
}


void
hybrid_free (void * vstate)
{
  hybrid_state_t * state = (hybrid_state_t *) vstate;

  gsl_matrix_free(state->lu);

  gsl_vector_int_free(state->permutation);
}


static const gsl_multiroot_fdfsolver_type hybrid_type =
{"hybrid",				/* name */
 sizeof (hybrid_state_t),
 &hybrid_init,
 &hybrid_iterate,
 &hybrid_free};

const gsl_multiroot_fdfsolver_type  * gsl_multiroot_fdfsolver_hybrid = &hybrid_type;