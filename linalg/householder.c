/* linalg/householder.c
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Gerard Jungman, Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <config.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_complex_math.h>

#include "gsl_linalg.h"

double
gsl_linalg_householder_transform (gsl_vector * v)
{
  /* replace v[0:n-1] with a householder vector (v[0:n-1]) and
     coefficient tau that annihilate v[1:n-1] */

  const size_t n = v->size ;

  if (n == 1)
    {
      return 0; /* tau = 0 */
    }
  else
    { 
      double alpha, beta, tau ;
      
      gsl_vector x = gsl_vector_subvector (v, 1, n - 1) ; 
      
      double xnorm = gsl_blas_dnrm2 (&x);
      
      if (xnorm == 0) 
        {
          return 0; /* tau = 0 */
        }
      
      alpha = gsl_vector_get (v, 0) ;
      beta = - (alpha >= 0 ? +1 : -1) * hypot(alpha, xnorm) ;
      tau = (beta - alpha) / beta ;
      
      gsl_blas_dscal (1.0 / (alpha - beta), &x);
      gsl_vector_set (v, 0, beta) ;
      
      return tau;
    }
}

int
gsl_linalg_householder_hm (double tau, const gsl_vector * v, gsl_matrix * A, gsl_vector * work)
{
  /* applies a householder transformation v,tau to matrix m */

  size_t i, j;

  if (tau == 0)
    return GSL_SUCCESS;

  /* w = A' v */

  for (i = 0; i < A->size2; i++)
    {
      double sum = gsl_matrix_get(A,0,i);  

      for (j = 1; j < A->size1; j++)  /* note, computed for v(0) = 1 above */
        {
          sum += gsl_matrix_get(A,j,i) * gsl_vector_get(v,j);
        }
      gsl_vector_set(work, i, sum);
    }

  /* A = A - v w' */

  for (j = 0; j < A->size2; j++) 
    {
      double wj = gsl_vector_get (work, j);
      double A0j = gsl_matrix_get (A, 0, j);
      gsl_matrix_set (A, 0, j, A0j - tau *  wj);
    }

  for (i = 1; i < A->size1; i++)
    {
      double vi = gsl_vector_get (v, i);

      for (j = 0; j < A->size2; j++) 
        {
          double wj = gsl_vector_get (work, j);
          double Aij = gsl_matrix_get (A, i, j);
          gsl_matrix_set (A, i, j, Aij - tau * vi * wj);
        }
    }

  return GSL_SUCCESS;
}

int
gsl_linalg_householder_hv (double tau, const gsl_vector * v, gsl_vector * w)
{
  /* applies a householder transformation v to vector w */
  size_t i;
  double d = 0;
 
  if (tau == 0)
    return GSL_SUCCESS ;

  /* d = v'w */

  d = gsl_vector_get(w,0);

  for (i = 1 ; i < v->size ; i++)
    {
      d += gsl_vector_get(v,i) * gsl_vector_get(w,i);
    }

  /* w = w - tau (v) (v'w) */
  
  {
    double w0 = gsl_vector_get (w,0);
    gsl_vector_set (w, 0, w0 - tau * d);
  }

  for (i = 1; i < v->size ; i++)
    {
      double wi = gsl_vector_get (w,i);
      double vi = gsl_vector_get (v,i);
      gsl_vector_set (w, i, wi - tau * vi * d);
    }

  return GSL_SUCCESS;
}


gsl_complex
gsl_linalg_complex_householder_transform (gsl_vector_complex * v)
{
  /* replace v[0:n-1] with a householder vector (v[0:n-1]) and
     coefficient tau that annihilate v[1:n-1] */

  const size_t n = v->size ;
  
  if (n == 1)
    {
      gsl_complex alpha = gsl_vector_complex_get (v, 0) ;      
      double absa = gsl_complex_abs (alpha);
      double beta_r = - (GSL_REAL(alpha) >= 0 ? +1 : -1) * absa ;

      gsl_complex tau;
      GSL_REAL(tau) = (beta_r - GSL_REAL(alpha)) / beta_r ;
      GSL_IMAG(tau) = - GSL_IMAG(alpha) / beta_r ;

      {
        gsl_complex beta = gsl_complex_rect (beta_r, 0.0);
        gsl_vector_complex_set (v, 0, beta) ;
      }
      
      return tau;
    }
  else
    { 
      gsl_complex tau ;
      double beta_r;

      gsl_vector_complex x = gsl_vector_complex_subvector (v, 1, n - 1) ; 
      gsl_complex alpha = gsl_vector_complex_get (v, 0) ;            
      double absa = gsl_complex_abs (alpha);
      double xnorm = gsl_blas_dznrm2 (&x);
      
      if (xnorm == 0 && GSL_IMAG(alpha) == 0) 
        {
          gsl_complex zero = gsl_complex_rect(0.0, 0.0);
          return zero; /* tau = 0 */
        }
      
      beta_r = - (GSL_REAL(alpha) >= 0 ? +1 : -1) * hypot(absa, xnorm) ;

      GSL_REAL(tau) = (beta_r - GSL_REAL(alpha)) / beta_r ;
      GSL_IMAG(tau) = - GSL_IMAG(alpha) / beta_r ;

      {
        gsl_complex amb = gsl_complex_sub_real(alpha, beta_r);
        gsl_complex s = gsl_complex_inverse(amb);
        gsl_blas_zscal (s, &x);
      }
      
      {
        gsl_complex beta = gsl_complex_rect (beta_r, 0.0);
        gsl_vector_complex_set (v, 0, beta) ;
      }
      
      return tau;
    }
}

int
gsl_linalg_complex_householder_hm (gsl_complex tau, const gsl_vector_complex * v, gsl_matrix_complex * A, gsl_vector_complex * work)
{
  /* applies a householder transformation v,tau to matrix m */

  size_t i, j;

  if (GSL_REAL(tau) == 0.0 && GSL_IMAG(tau) == 0.0)
    {
      return GSL_SUCCESS;
    }

  /* w =   (v' A)^T */

  for (i = 0; i < A->size2; i++)
    {
      gsl_complex sum = gsl_matrix_complex_get(A,0,i);  

      for (j = 1; j < A->size1; j++)  /* note, computed for v(0) = 1 above */
        {
          gsl_complex Aji = gsl_matrix_complex_get(A,j,i);
          gsl_complex vj = gsl_vector_complex_get(v,j);
          gsl_complex Av = gsl_complex_mul (Aji, gsl_complex_conjugate(vj));
          sum = gsl_complex_add (sum, Av);
        }

      gsl_vector_complex_set(work, i, sum);
    }

  /* A = A - v w^T */

  for (j = 0; j < A->size2; j++) 
    {
      gsl_complex wj = gsl_vector_complex_get (work, j);
      gsl_complex A0j = gsl_matrix_complex_get (A, 0, j);
      gsl_complex tauwj = gsl_complex_mul (tau, wj);
      gsl_complex Atw = gsl_complex_sub (A0j, tauwj);
      /* store A0j - tau  * wj */
      gsl_matrix_complex_set (A, 0, j, Atw);
    }

  for (i = 1; i < A->size1; i++)
    {
      gsl_complex vi = gsl_vector_complex_get (v, i);

      for (j = 0; j < A->size2; j++) 
        {
          gsl_complex wj = gsl_vector_complex_get (work, j);
          gsl_complex Aij = gsl_matrix_complex_get (A, i, j);
          gsl_complex vw = gsl_complex_mul(vi, wj);
          gsl_complex tauvw = gsl_complex_mul(tau, vw);
          gsl_complex Atwv = gsl_complex_sub (Aij, tauvw);
          /* store Aij - tau * vi * wj */
          gsl_matrix_complex_set (A, i, j, Atwv);
        }
    }

  return GSL_SUCCESS;
}
