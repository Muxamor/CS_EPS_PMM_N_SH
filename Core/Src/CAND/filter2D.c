#include "stm32l4xx.h"
#include "CAND/filter2D.h"


typeFiltrCoeff FiltrCoeff[MAX_FILTR_CHAN];


int16_t Filtr2Step( typeEnumFiltrChan chan, int16_t  v ){

  int32_t y, x2, x1, x0;

  x2 = (int32_t)((((int64_t)v << 16) * IIR_A2) >> 16);
  x1 = (int32_t)(((int64_t)FiltrCoeff[chan].a[1] * IIR_A1) >> 16);
  x0 = (int32_t)(((int64_t)FiltrCoeff[chan].a[0] * IIR_A0) >> 16);

  //y = ((x2 + x1 + x0) != -1) ? (x2 + x1 + x0) : 0;
  y = x2 + x1 + x0;
  FiltrCoeff[chan].a[0] = FiltrCoeff[chan].a[1];
  FiltrCoeff[chan].a[1] = y;

  y = y >> 16;
  y = ( y != -1) ? y : 0;

  return (int16_t)(y);
}


uint16_t Filtr2StepUnsigned( typeEnumFiltrChan chan, uint16_t  v ){

  uint32_t x2, x1, x0;
  uint32_t y;

  x2 = (uint32_t)((((uint64_t)v << 16) * IIR_A2) >> 16);
  x1 = (uint32_t)(((uint64_t)FiltrCoeff[chan].a[1] * IIR_A1) >> 16);
  x0 = (uint32_t)(((uint64_t)FiltrCoeff[chan].a[0] * IIR_A0) >> 16);

  y = ((x2 + x1 + x0) < 0xFFFF0000) ? (x2 + x1 + x0) : 0;

  FiltrCoeff[chan].a[0] = FiltrCoeff[chan].a[1];
  FiltrCoeff[chan].a[1] = y;

  y = (uint16_t)(y >> 16);

  return y;
}




