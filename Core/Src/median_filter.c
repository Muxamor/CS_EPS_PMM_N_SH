#include "stm32l4xx.h"
#include "median_filter.h"
//--------------------------------------------------------------------------------------------------
//------------------------functions for quick median search-----------------------------------------
//--------------------------------------------------------------------------------------------------
 void swap(int8_t * a, int8_t * b){
   int8_t c = *a;
   *a = *b;
   *b = c;
}

 uint16_t partition(int8_t * mas, uint16_t l, uint16_t r, uint16_t pivot) {
  int16_t x;
  uint16_t j, i;

  if (l != r)
    swap(&mas[pivot], &mas[r]);  //swap(mas[l + rand() % (r - l)], mas[r]);

  x = mas[r];

  for (j = l, i = l - 1; j <= r; j ++) {
    if (mas[j] <= x)
      swap(&mas[++i], &mas[j]);
  }
  return i;
}

 int8_t GetPosition(int8_t * mas, uint16_t size, uint16_t n) {
  uint16_t pos, l = 0, r = size - 1;

  for(;;) {
    pos = partition(mas, l, r, l);
    if (pos < n)
      l = pos + 1;
    else if (pos > n)
      r = pos - 1;
    else return mas[n];
  }
}

int8_t GetMedian(int8_t * mas, uint16_t size){
   if(size % 2){
      return GetPosition(mas, size, size / 2);
   } else {
      return ( GetPosition(mas, size, size / 2) + GetPosition(mas, size, size / 2 - 1)) / 2;
   }
}

//--------------------------------------------------------------------------------------------------
//
//void MedianTest(void){
//   int8_t  tst1[] = {0, 0, 0, 0, 0, 0, 0};                        //0
//   int8_t  tst2[] = {3, 3, 3, 3, 3, 3, 3};                        //3
//   int8_t  tst3[] = {0, 100};                                     //50
//   int8_t  tst4[] = {100, 100, 100, -100, -100, -100, -90, 10};   //-40
//   int8_t  tst5[] = {127, 127, 127, -128, -128, -128, 0, 2};      //1
//   int8_t  tst6[] = {10, 10, 10, 10, -10, -10, -10, -10, -1, 1};  //0
//   int8_t  tst7[] = {10, 10, 10, 10, -10, -10, -10, -10, 1, 1};   //1
//   int8_t  tst8[] = {10, 10, 10, 10, -10, -10, -10, -10, -1};     //-1
//
//   int8_t result;
//
//   result = GetMedian(tst1, sizeof(tst1) / sizeof(tst1[0]));//    0
//   result = GetMedian(tst2, sizeof(tst2) / sizeof(tst2[0]));//    3
//   result = GetMedian(tst3, sizeof(tst3) / sizeof(tst3[0]));//    50
//   result = GetMedian(tst4, sizeof(tst4) / sizeof(tst4[0]));//    -40
//   result = GetMedian(tst5, sizeof(tst5) / sizeof(tst5[0]));//    1
//   result = GetMedian(tst6, sizeof(tst6) / sizeof(tst6[0]));//    0
//   result = GetMedian(tst7, sizeof(tst7) / sizeof(tst7[0]));//    1
//   result = GetMedian(tst8, sizeof(tst8) / sizeof(tst8[0]));//    -1
//
//   result = GetMedian(tst1, sizeof(tst1) / sizeof(tst1[0]));
//}

//--------------------------------------------------------------------------------------------------
