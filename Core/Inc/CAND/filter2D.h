#ifndef CS_EPS_PMM_N_SH_V_FILTER2D_H
#define CS_EPS_PMM_N_SH_V_FILTER2D_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CHF_U1,
    CHF_U2,
    CHF_U3,
    CHF_U4,
    CHF_U5,
    CHF_U6,
    CHF_I1,
    CHF_I2,
    CHF_I3,
    CHF_I4,
    CHF_I5,
    CHF_I6
}typeEnumFiltrChan;

#define MAX_FILTR_CHAN   12

/*постоянная времени ~25 точек, для интервалов 0.5сек это 12сек */
#define IIR_A2    1120
#define IIR_A1    113154
#define IIR_A0    -48738


typedef struct {
    int32_t a[2];
}typeFiltrCoeff;



#ifdef __cplusplus
}
#endif

#endif //CS_EPS_PMM_N_SH_V_FILTER2D_H
