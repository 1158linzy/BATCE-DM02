#ifndef NV_SAVE_H_
#define NV_SAVE_H_

#include "hi_types_base.h"
#define  HI_NV_SYS_SAVE_STATE  0x41

typedef struct {
    hi_u8 led_state;
}hi_sys_led_bulb;


typedef struct {
	unsigned int switch_on;
	unsigned int faltDetection_code;
	unsigned int faltDetection_status;
} t_device_info;
// extern t_device_info hilink_g_device_info();
// extern void hilink_set_g_device_info(	unsigned int switch_on,
// 	unsigned int faltDetection_code,
// 	unsigned int faltDetection_status);
// extern t_device_info hilink_get_device_info(void);

extern t_device_info g_device_info;

#endif