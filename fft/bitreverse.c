#include <config.h>
#include <gsl_fft.h>

#include "fft.h"
#include "bitreverse.h"

int 
fft_bitreverse_order_complex (double data[], 
			      const size_t stride,
			      const size_t n)
{
  /* This is the Goldrader bit-reversal algorithm */

  size_t i;
  size_t j = 0;

  for (i = 0; i < n - 1; i++)
    {
      size_t k = n / 2 ;

      if (i < j)
	{
	  const double tmp_real = REAL(data,stride,i);
	  const double tmp_imag = IMAG(data,stride,i);
	  REAL(data,stride,i) = REAL(data,stride,j);
	  IMAG(data,stride,i) = IMAG(data,stride,j);
	  REAL(data,stride,j) = tmp_real;
	  IMAG(data,stride,j) = tmp_imag;
	}

      while (k <= j) 
	{
	  j = j - k ;
	  k = k / 2 ;
	}

      j += k ;
    }

  return 0;
}


int 
fft_bitreverse_order_real (double data[], 
			      const size_t stride,
			      const size_t n)
{
  /* This is the Goldrader bit-reversal algorithm */

  size_t i;
  size_t j = 0;

  for (i = 0; i < n - 1; i++)
    {
      size_t k = n / 2 ;

      if (i < j)
	{
	  const double tmp = VECTOR(data,stride,i);
	  VECTOR(data,stride,i) = REAL(data,stride,j);
	  VECTOR(data,stride,j) = tmp;
	}

      while (k <= j) 
	{
	  j = j - k ;
	  k = k / 2 ;
	}

      j += k ;
    }

  return 0;
}

