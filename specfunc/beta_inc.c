/* Author:  G. Jungman
 * RCS:     $Id$
 */
#include <config.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_log.h>
#include <gsl/gsl_sf_exp.h>
#include "gsl_sf_gamma.h"



static
int
beta_cont_frac(
  const double a,
  const double b,
  const double x,
  gsl_sf_result * result
  )
{
  const unsigned int max_iter = 512;        /* control iterations      */
  const double cutoff = 2.0 * GSL_DBL_MIN;  /* control the zero cutoff */
  unsigned int iter_count = 0;
  double cf;

  /* standard initialization for continued fraction */
  double num_term = 1.0;
  double den_term = 1.0 - (a+b)*x/(a+1.0);
  if (fabs(den_term) < cutoff) den_term = cutoff;
  den_term = 1.0/den_term;
  cf = den_term;

  while(iter_count < max_iter) {
    const int k  = iter_count + 1;
    double coeff = k*(b-k)*x/(((a-1.0)+2*k)*(a+2*k));
    double delta_frac;

    /* first step */
    den_term = 1.0 + coeff*den_term;
    num_term = 1.0 + coeff/num_term;
    if(fabs(den_term) < cutoff) den_term = cutoff;
    if(fabs(num_term) < cutoff) num_term = cutoff;
    den_term  = 1.0/den_term;

    delta_frac = den_term * num_term;
    cf *= delta_frac;

    coeff = -(a+k)*(a+b+k)*x/((a+2*k)*(a+2*k+1.0));

    /* second step */
    den_term = 1.0 + coeff*den_term;
    num_term = 1.0 + coeff/num_term;
    if(fabs(den_term) < cutoff) den_term = cutoff;
    if(fabs(num_term) < cutoff) num_term = cutoff;
    den_term = 1.0/den_term;

    delta_frac = den_term*num_term;
    cf *= delta_frac;

    if(fabs(delta_frac-1.0) < 2.0*GSL_DBL_EPSILON) break;

    ++iter_count;
  }

  result->val = cf;
  result->err = iter_count * 4.0 * GSL_DBL_EPSILON * fabs(cf);

  if(iter_count >= max_iter)
    return GSL_EMAXITER;
  else
    return GSL_SUCCESS;
}



/*-*-*-*-*-*-*-*-*-*-*-* (semi)Private Implementations *-*-*-*-*-*-*-*-*-*-*-*/

int
gsl_sf_beta_inc_impl(
  const double a,
  const double b,
  const double x,
  gsl_sf_result * result
  )
{
  if(a <= 0.0 || b <= 0.0 || x < 0.0 || x > 1.0) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
  else if(x == 0.0) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_SUCCESS;
  }
  else if(x == 1.0) {
    result->val = 1.0;
    result->err = 0.0;
    return GSL_SUCCESS;
  }
  else {
    gsl_sf_result ln_beta;
    gsl_sf_result ln_x;
    gsl_sf_result ln_1mx;
    gsl_sf_result prefactor;
    const int stat_ln_beta = gsl_sf_lnbeta_impl(a, b, &ln_beta);
    const int stat_ln_1mx = gsl_sf_log_1plusx_impl(-x, &ln_1mx);
    const int stat_ln_x = gsl_sf_log_impl(x, &ln_x);
    const int stat_ln = GSL_ERROR_SELECT_3(stat_ln_beta, stat_ln_1mx, stat_ln_x);

    const double ln_pre_val = -ln_beta.val + a * ln_x.val + b * ln_1mx.val;
    const double ln_pre_err =  ln_beta.err + fabs(a*ln_x.err) + fabs(b*ln_1mx.err);
    const int stat_exp = gsl_sf_exp_err_impl(ln_pre_val, ln_pre_err, &prefactor);

    if(stat_ln != GSL_SUCCESS) {
      result->val = 0.0;
      result->err = 0.0;
      return GSL_ESANITY;
    }

    if(x < (a + 1.0)/(a+b+2.0)) {
      /* Apply continued fraction directly. */
      gsl_sf_result cf;
      const int stat_cf = beta_cont_frac(a, b, x, &cf);
      int stat;
      result->val = prefactor.val * cf.val / a;
      result->err = (fabs(prefactor.err * cf.val) + fabs(prefactor.val * cf.err))/a;

      stat = GSL_ERROR_SELECT_2(stat_exp, stat_cf);
      if(result->val == 0.0 && stat == GSL_SUCCESS) {
        return GSL_EUNDRFLW;
      }
      else {
        return stat;
      }
    }
    else {
      /* Apply continued fraction after hypergeometric transformation. */
      gsl_sf_result cf;
      const int stat_cf = beta_cont_frac(b, a, 1.0-x, &cf);
      int stat;
      const double term = prefactor.val * cf.val / b;
      result->val  = 1.0 - term;
      result->err  = fabs(prefactor.err * cf.val)/b;
      result->err += fabs(prefactor.val * cf.err)/b;
      result->err += 2.0 * GSL_DBL_EPSILON * (1.0 + fabs(term));
      stat = GSL_ERROR_SELECT_2(stat_exp, stat_cf);
      if(result->val == 0.0 && stat == GSL_SUCCESS) {
        return GSL_EUNDRFLW;
      }
      else {
        return stat;
      }
    }
  }
}



/*-*-*-*-*-*-*-*-*-*-*-* Functions w/ Error Handling *-*-*-*-*-*-*-*-*-*-*-*/


int
gsl_sf_beta_inc_e(
  const double a,
  const double b,
  const double x,
  gsl_sf_result * result
  )
{
  int status = gsl_sf_beta_inc_impl(a, b, x, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_beta_inc_e", status);
  }
  return status;
}
