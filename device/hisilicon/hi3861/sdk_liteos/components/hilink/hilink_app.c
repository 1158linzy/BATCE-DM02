#include "hilink_device.h"
#include "hilink_log.h"
#include <stdlib.h>
#include "nv_save.h"
#include "hi_nv.h"

#define M2M_NO_ERROR 0
#define M2M_SVC_RPT_CREATE_PAYLOAD_ERR -201

#ifndef INVALID_PACKET
#define INVALID_PACKET -87
#endif

/* 设备服务Service 数组索引 */
#define SRV_IDX_SWITCH 0

t_device_info g_device_info = { 0 };

// t_device_info hilink_get_device_info(void)
// {

// 	return g_device_info;
// }

// void hilink_set_g_device_info(	unsigned int switch_on,
// 	unsigned int faltDetection_code,
// 	unsigned int faltDetection_status)
// {
// 	if (switch_on != 0) {
// 	g_device_info.switch_on = switch_on;
// 	}
// }
int hi_nv_state_write(int state)
{
	hi_u32 ret;
	hi_sys_led_bulb cfg11;
	cfg11.led_state = state;
	printf("led_state:%d\r\n", cfg11.led_state);

	ret = hi_nv_write(HI_NV_SYS_SAVE_STATE, &cfg11, sizeof(cfg11), 0);
	if (ret != HI_ERR_SUCCESS)
	{
		printf("nv write fail\r\n");
		return;
	}
}
int handle_put_switch(const char *svc_id, const char *payload, unsigned int len) {

	void *pJson = hilink_json_parse(payload);
	if (pJson == NULL) {
		printf("JSON parse failed in PUT cmd: ID-%s \r\n", svc_id);
		return INVALID_PACKET;
	}
	bool on;
	bool *on_p = NULL;
	if (hilink_json_get_number_value(pJson, "on", &on) == 0
			&& (on == 0 || on == 1)) {
		on_p = &on;
	}
	// int g_device_info_state=hilink_get_device_info().switch_on;
	// ///////
	// t_device_info g_device_info_kk = { 0 };
	// g_device_info_kk.switch_on = *on_p;
	// hilink_set_g_device_info(g_device_info_kk);
	////////
	g_device_info.switch_on = *on_p;
	if (pJson != NULL) {
		hilink_json_delete(pJson);
	}
	printf("handle func:%s, sid:%s \r\n", __FUNCTION__, svc_id);
	printf("g_device_info.switch_on:%d\r\n", g_device_info.switch_on);
	hi_nv_state_write(g_device_info.switch_on);
	return M2M_NO_ERROR;
}
int handle_get_switch(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len) {

	bool on = g_device_info.switch_on;
	*out_len = 20;
	*out = (char*) hilink_malloc(*out_len);
	if (NULL == *out) {
		printf("malloc failed in GET cmd: ser %s in GET cmd", svc_id);
		return INVALID_PACKET;
	}
	*out_len = sprintf(*out, "{\"on\":%d}", on);
	printf("hilink_device_ctr.c :%d %s svcId:%s, out:%s\r\n", __LINE__,
			__FUNCTION__, svc_id, *out);

	return M2M_NO_ERROR;
}

typedef int (*handle_put_func)(const char *svc_id, const char *payload, unsigned int len);
typedef int (*handle_get_func)(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len);

typedef struct {
	char *sid;
	handle_put_func putFunc;
	handle_get_func getFunc;
} HANDLE_SVC_INFO;


HANDLE_SVC_INFO g_device_profile[] =
{
	{ "switch", handle_put_switch, handle_get_switch },
};

int g_device_profile_count = sizeof(g_device_profile) / sizeof(HANDLE_SVC_INFO);

static HANDLE_SVC_INFO* find_handle(const char *svc_id) {

	for (int i = 0; i < g_device_profile_count; i++)
	{
		HANDLE_SVC_INFO handle = g_device_profile[i];

		if (strcmp(handle.sid, svc_id) == 0)
		{
			return &g_device_profile[i];
		}
	}
	return NULL;
}
int handle_put_cmd(const char *svc_id, const char *payload, unsigned int len) {

	HANDLE_SVC_INFO *handle = find_handle(svc_id);
	if (handle == NULL) {
		printf("no service to handle put cmd : %s \r\n", svc_id);
		return INVALID_PACKET;
	}
	handle_put_func function = handle->putFunc;
	if (function == NULL) {
		printf("put function is null for %s \r\n", svc_id);
		return INVALID_PACKET;
	}

	return function(svc_id, payload, len);
}
int handle_get_cmd(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len) {

	HANDLE_SVC_INFO *handle = find_handle(svc_id);
	if (handle == NULL) {
		printf("no service to handle get cmd : %s \r\n", svc_id);
		return INVALID_PACKET;
	}
	handle_get_func function = handle->getFunc;
	if (function == NULL) {
		printf("get function is null for %s \r\n", svc_id);
		return INVALID_PACKET;
	}

	return function(svc_id, in, in_len, out, out_len);
}
int hilink_put_char_state(const char *svc_id, const char *payload, unsigned int len) {

	int err = M2M_NO_ERROR;
	if (svc_id == NULL) {
		hilink_error("empty service ID in PUT cmd");
		return M2M_SVC_RPT_CREATE_PAYLOAD_ERR;
	}
	if (payload == NULL) {
		hilink_error("empty payload in PUT cmd");
		return M2M_SVC_RPT_CREATE_PAYLOAD_ERR;
	}
	hilink_debug("start handle PUT cmd: ID-%s", svc_id);
	err = handle_put_cmd(svc_id, payload, len);
	hilink_debug("handle PUT cmd end: ID-%s, ret-%d", svc_id, err);

	return err;
}
int hilink_get_char_state(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len) {

	int err = M2M_NO_ERROR;
	if (svc_id == NULL) {
		hilink_error("empty service ID in GET cmd");
		return M2M_SVC_RPT_CREATE_PAYLOAD_ERR;
	}
	hilink_info("start process GET cmd: ID - %s", svc_id);
	err = handle_get_cmd(svc_id, in, in_len, out, out_len);
	hilink_debug("end process GET cmd: ID - %s, ret - %d", svc_id, err);

	return err;
}
//上报状态
//int hilink_report_state(const char *svcId, const char *payload, unsigned int len)
int hilink_report_state()
{
	int err = M2M_NO_ERROR;
	int on = g_device_info.switch_on;
	int out_len = 0;
	char out[20];
	memset(out,  0, sizeof(char) * 20);
	out_len = snprintf(out, 20, "{\"on\":%d}", on);
	printf("out:%s\r\n", out);
	err = HILINK_ReportCharState(g_device_profile[SRV_IDX_SWITCH].sid, out, strlen(out));
	return err;
}

