#ifndef GSL_MULTIROOTS_H
#define GSL_MULTIROOTS_H

#include <stdlib.h>
#include <gsl_math.h>
#include <gsl_vector.h>
#include <gsl_matrix.h>

/* Definition of vector-valued functions with parameters based on gsl_vector */

struct gsl_multiroot_function_struct
{
  int (* f) (const gsl_vector * x, void * params, gsl_vector * f);
  size_t n;
  void * params;
};

typedef struct gsl_multiroot_function_struct gsl_multiroot_function ;

#define GSL_MULTIROOT_FN_EVAL(F,x,y) (*((F)->f))(x,(F)->params,(y))

int gsl_multiroots_fdjacobian (gsl_multiroot_function * F,
                               const gsl_vector * x, const gsl_vector * f,
                               double epsrel, gsl_matrix * jacobian);


typedef struct
  {
    const char *name;
    size_t size;
    int (*set) (void *state, gsl_multiroot_function * f, gsl_vector * x);
    int (*iterate) (void *state, gsl_multiroot_function * f, gsl_vector * x);
  }
gsl_multiroot_fsolver_type;

typedef struct
  {
    const gsl_multiroot_fsolver_type * type;
    gsl_multiroot_function * function ;
    gsl_vector * x ;
    gsl_vector * f ;
    void *state;
  }
gsl_multiroot_fsolver;

gsl_multiroot_fsolver *
gsl_multiroot_fsolver_alloc (const gsl_multiroot_fsolver_type * T, 
                             gsl_multiroot_function * f, gsl_vector * x);
void gsl_multiroot_fsolver_free (gsl_multiroot_fsolver * s);

int gsl_multiroot_fsolver_set (gsl_multiroot_fsolver * s, 
                               gsl_multiroot_function * f, gsl_vector * x);

int gsl_multiroot_fsolver_iterate (gsl_multiroot_fsolver * s);

const char * gsl_multiroot_fsolver_name (const gsl_multiroot_fsolver * s);
double gsl_multiroot_fsolver_root (const gsl_multiroot_fsolver * s);
gsl_interval gsl_multiroot_fsolver_interval (const gsl_multiroot_fsolver * s);


/* Definition of vector-valued functions and gradient with parameters
   based on gsl_vector */

struct gsl_multiroot_function_fdf_struct
{
  int (* f) (const gsl_vector * x, void * params, gsl_vector * f);
  int (* df) (const gsl_vector * x, void * params, gsl_matrix * df);
  int (* fdf) (const gsl_vector * x, void * params, gsl_vector * f, gsl_matrix *fdf);
  size_t n;
  void * params;
};

typedef struct gsl_multiroot_function_fdf_struct gsl_multiroot_function_fdf ;

#define GSL_MULTIROOT_FN_EVAL_F(F,x,y) (*((F)->f)(x,(F)->params,(y)))
#define GSL_MULTIROOT_FN_EVAL_DF(F,x,dy) (*((F)->df)(x,(F)->params,(dy)))
#define GSL_MULTIROOT_FN_EVAL_F_DF(F,x,y,dy) (*((F)->fdf)(x,(F)->params,(y),(dy)))

typedef struct
  {
    const char *name;
    size_t size;
    int (*set) (void *state, gsl_multiroot_function_fdf * f, gsl_vector * x);
    int (*iterate) (void *state, gsl_multiroot_function_fdf * f, gsl_vector * x);
  }
gsl_multiroot_fdfsolver_type;

typedef struct
  {
    const gsl_multiroot_fdfsolver_type * type;
    gsl_function_fdf * fdf ;
    gsl_vector * x;
    gsl_vector * f;
    gsl_matrix * g;
    void *state;
  }
gsl_multiroot_fdfsolver;


gsl_multiroot_fdfsolver *
gsl_multiroot_fdfsolver_alloc (const gsl_multiroot_fdfsolver_type * T, 
                               gsl_function_fdf * fdf, double root);

int
gsl_multiroot_fdfsolver_set (gsl_multiroot_fdfsolver * s, 
                             gsl_function_fdf * fdf, double root);

int
gsl_multiroot_fdfsolver_iterate (gsl_multiroot_fdfsolver * s);

void
gsl_multiroot_fdfsolver_free (gsl_multiroot_fdfsolver * s);

const char * gsl_multiroot_fdfsolver_name (const gsl_multiroot_fdfsolver * s);
double gsl_multiroot_fdfsolver_root (const gsl_multiroot_fdfsolver * s);

int
gsl_multiroot_test_interval (gsl_interval x, double epsabs, double epsrel);

int
gsl_multiroot_test_residual (double f, double epsabs);

int
gsl_multiroot_test_delta (double x1, double x0, double epsabs, double epsrel);

extern const gsl_multiroot_fsolver_type  * gsl_multiroot_fsolver_bisection;
extern const gsl_multiroot_fdfsolver_type  * gsl_multiroot_fdfsolver_newton;

#endif /* GSL_MULTIROOTS_H */