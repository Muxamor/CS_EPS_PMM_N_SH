#include "stm32l4xx.h"
#include "CAND/filter2D.h"


typeFiltrCoeff FiltrCoeff[MAX_FILTR_CHAN];


int16_t Filtr2Step( typeEnumFiltrChan chan, int16_t  v ){

  int32_t y, x2, x1, x0;

  x2 = (int32_t)((((int64_t)v << 16) * IIR_A2) >> 16);
  x1 = (int32_t)(((int64_t)FiltrCoeff[chan].a[1] * IIR_A1) >> 16);
  x0 = (int32_t)(((int64_t)FiltrCoeff[chan].a[0] * IIR_A0) >> 16);

  y = x2 + x1 + x0;

  FiltrCoeff[chan].a[0] = FiltrCoeff[chan].a[1];
  FiltrCoeff[chan].a[1] = y;

  return (int16_t)(y >> 16);
}



