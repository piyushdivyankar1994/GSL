/* Author:  G. Jungman
 * RCS:     $Id$
 */
#ifndef __GSL_SF_GAMMA_H__
#define __GSL_SF_GAMMA_H__

#include <gsl/gsl_sf_result.h>

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS


/* Log[Gamma(x)], x not a negative integer
 * Uses real Lanczos method.
 * Returns the real part of Log[Gamma[x]] when x < 0,
 * i.e. Log[|Gamma[x]|].
 *
 * exceptions: GSL_EDOM, GSL_EROUND
 */
int gsl_sf_lngamma_impl(double x, gsl_sf_result * result);
int gsl_sf_lngamma_e(const double x, gsl_sf_result * result);


/* Log[Gamma(x)], x not a negative integer
 * Uses real Lanczos method. Determines
 * the sign of Gamma[x] as well as Log[|Gamma[x]|] for x < 0.
 * So Gamma[x] = sgn * Exp[result_lg].
 *
 * exceptions: GSL_EDOM, GSL_EROUND
 */
int gsl_sf_lngamma_sgn_impl(double x, gsl_sf_result * result_lg, double *sgn);
int gsl_sf_lngamma_sgn_e(const double x, gsl_sf_result * result_lg, double * sgn);


/* Gamma(x), x not a negative integer
 * Uses real Lanczos method.
 *
 * exceptions: GSL_EDOM, GSL_EOVRFLW, GSL_EROUND
 */
int gsl_sf_gamma_impl(const double x, gsl_sf_result * result);
int gsl_sf_gamma_e(const double x, gsl_sf_result * result);


/* Regulated Gamma Function, x > 0
 * Gamma^*(x) = Gamma(x)/(Sqrt[2Pi] x^(x-1/2) exp(-x))
 *            = (1 + 1/(12x) + ...),  x->Inf
 * A useful suggestion of Temme.
 *
 * exceptions: GSL_EDOM
 */
int gsl_sf_gammastar_impl(const double x, gsl_sf_result * result);
int gsl_sf_gammastar_e(const double x, gsl_sf_result * result);


/* 1/Gamma(x)
 * Uses real Lanczos method.
 *
 * exceptions: GSL_EUNDRFLW, GSL_EROUND
 */
int gsl_sf_gammainv_impl(const double x, gsl_sf_result * result);
int gsl_sf_gammainv_e(const double x, gsl_sf_result * result);


/* Log[Gamma(z)] for z complex, z not a negative integer
 * Uses complex Lanczos method. Note that the phase part (arg)
 * is not well-determined when |z| is very large, due
 * to inevitable roundoff in restricting to (-Pi,Pi].
 * This will raise the GSL_ELOSS exception when it occurs.
 * The absolute value part (lnr), however, never suffers.
 *
 * Calculates:
 *   lnr = log|Gamma(z)|
 *   arg = arg(Gamma(z))  in (-Pi, Pi]
 *
 * exceptions: GSL_EDOM, GSL_ELOSS
 */
int gsl_sf_lngamma_complex_impl(double zr, double zi, gsl_sf_result * lnr, gsl_sf_result * arg);
int gsl_sf_lngamma_complex_e(double zr, double zi, gsl_sf_result * lnr, gsl_sf_result * arg);


/* x^n / n!
 *
 * x >= 0.0, n >= 0
 * exceptions: GSL_EDOM, GSL_EOVRFLW, GSL_EUNDRFLW
 */
int gsl_sf_taylorcoeff_impl(const int n, const double x, gsl_sf_result * result);
int gsl_sf_taylorcoeff_e(const int n, const double x, gsl_sf_result * result);


/* n!
 *
 * exceptions: GSL_EDOM, GSL_OVRFLW
 */
int gsl_sf_fact_impl(const unsigned int n, gsl_sf_result * result);
int gsl_sf_fact_e(const unsigned int n, gsl_sf_result * result);


/* n!! = n(n-2)(n-4) ... 
 *
 * exceptions: GSL_EDOM, GSL_OVRFLW
 */
int gsl_sf_doublefact_impl(const unsigned int n, gsl_sf_result * result);
int gsl_sf_doublefact_e(const unsigned int n, gsl_sf_result * result);


/* log(n!) 
 * Faster than ln(Gamma(n+1)) for n < 170; defers for larger n.
 *
 * exceptions: none
 */
int gsl_sf_lnfact_impl(const unsigned int n, gsl_sf_result * result);
int gsl_sf_lnfact_e(const unsigned int n, gsl_sf_result * result);


/* log(n!!) 
 *
 * exceptions: none
 */
int gsl_sf_lndoublefact_impl(const unsigned int n, gsl_sf_result * result);
int gsl_sf_lndoublefact_e(const unsigned int n, gsl_sf_result * result);


/* log(n choose m)
 *
 * exceptions: GSL_EDOM 
 */
int gsl_sf_lnchoose_impl(unsigned int n, unsigned int m, gsl_sf_result * result);
int gsl_sf_lnchoose_e(unsigned int n, unsigned int m, gsl_sf_result * result);


/* n choose m
 *
 * exceptions: GSL_EDOM, GSL_EOVRFLW
 */
int gsl_sf_choose_impl(unsigned int n, unsigned int m, gsl_sf_result * result);
int gsl_sf_choose_e(unsigned int n, unsigned int m, gsl_sf_result * result);


/* Logarithm of Pochammer (Apell) symbol
 *   log( (a)_x )
 *   where (a)_x := Gamma[a + x]/Gamma[a]
 *
 * a > 0, a+x > 0
 *
 * exceptions:  GSL_EDOM
 */
int gsl_sf_lnpoch_impl(const double a, const double x, gsl_sf_result * result);
int gsl_sf_lnpoch_e(const double a, const double x, gsl_sf_result * result);


/* Logarithm of Pochammer (Apell) symbol, with sign information.
 *   result = log( |(a)_x| )
 *   sgn    = sgn( (a)_x )
 *   where (a)_x := Gamma[a + x]/Gamma[a]
 *
 * a != neg integer, a+x != neg integer
 *
 * exceptions:  GSL_EDOM
 */
int gsl_sf_lnpoch_sgn_impl(const double a, const double x, gsl_sf_result * result, double * sgn);
int gsl_sf_lnpoch_sgn_e(const double a, const double x, gsl_sf_result * result, double * sgn);


/* Pochammer (Apell) symbol
 *   (a)_x := Gamma[a + x]/Gamma[x]
 *
 * a != neg integer, a+x != neg integer
 *
 * exceptions:  GSL_EDOM, GSL_EOVRFLW
 */
int gsl_sf_poch_impl(const double a, const double x, gsl_sf_result * result);
int gsl_sf_poch_e(const double a, const double x, gsl_sf_result * result);


/* Relative Pochammer (Apell) symbol
 *   ((a,x) - 1)/x
 *   where (a,x) = (a)_x := Gamma[a + x]/Gamma[a]
 *
 * exceptions:  GSL_EDOM
 */
int gsl_sf_pochrel_impl(const double a, const double x, gsl_sf_result * result);
int gsl_sf_pochrel_e(const double a, const double x, gsl_sf_result * result);


/* Normalized Incomplete Gamma Function
 *
 * Q(a,x) = 1/Gamma(a) Integral[ t^(a-1) e^(-t), {t,x,Infinity} ]
 *
 * a > 0, x >= 0
 *
 * exceptions: GSL_EDOM
 */
int gsl_sf_gamma_inc_Q_impl(const double a, const double x, gsl_sf_result * result);
int gsl_sf_gamma_inc_Q_e(const double a, const double x, gsl_sf_result * result);


/* Complementary Normalized Incomplete Gamma Function
 *
 * P(a,x) = 1/Gamma(a) Integral[ t^(a-1) e^(-t), {t,0,x} ]
 *
 * a > 0, x >= 0
 *
 * exceptions: GSL_EDOM
 */
int gsl_sf_gamma_inc_P_impl(const double a, const double x, gsl_sf_result * result);
int gsl_sf_gamma_inc_P_e(const double a, const double x, gsl_sf_result * result);


/* Logarithm of Beta Function
 * Log[B(a,b)]
 *
 * a > 0, b > 0
 * exceptions: GSL_EDOM
 */
int gsl_sf_lnbeta_impl(const double a, const double b, gsl_sf_result * result);
int gsl_sf_lnbeta_e(const double a, const double b, gsl_sf_result * result);


/* Beta Function
 * B(a,b)
 *
 * a > 0, b > 0
 * exceptions: GSL_EDOM, GSL_EOVRFLW, GSL_EUNDRFLW
 */
int gsl_sf_beta_impl(const double a, const double b, gsl_sf_result * result);
int gsl_sf_beta_e(const double a, const double b, gsl_sf_result * result);


/* Normalized Incomplete Beta Function
 * B_x(a,b)/B(a,b)
 *
 * a > 0, b > 0, 0 <= x <= 1
 * exceptions: GSL_EDOM, GSL_EUNDRFLW
 */
int gsl_sf_beta_inc_impl(const double a, const double b, double x, gsl_sf_result * result);
int gsl_sf_beta_inc_e(const double a, const double b, double x, gsl_sf_result * result);


/* The maximum x such that gamma(x) is not
 * considered an overflow.
 */
#define GSL_SF_GAMMA_XMAX  171.0


__END_DECLS

#endif /* __GSL_SF_GAMMA_H__ */
