#ifndef INC_PAM_PAM_INIT_H_
#define INC_PAM_PAM_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif


ErrorStatus PAM_init(_PAM *pam_ptr);

//ErrorStatus PAM_init_SP_Temp_sensors(_PAM *pam_ptr, uint8_t num_ch);

#ifdef __cplusplus
}
#endif

#endif /* INC_PAM_PAM_INIT_H_ */
