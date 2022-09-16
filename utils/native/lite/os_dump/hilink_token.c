#include "hilink_token.h"

#define CONFIG_HILINK	//外部程序检测不到内部配置，需要手动使能

#ifdef  CONFIG_HILINK
#define HILINK_TOKEN_LEN	151

hi_u32 cmd_get_systoken(hi_s32 argc, const hi_char *argv[]) {

	extern int HalReadToken(char *token, unsigned int len);

	hi_unref_param(argc);
	hi_unref_param(argv);

	char hilink_token[HILINK_TOKEN_LEN] = { 0 };

	if (HalReadToken(hilink_token, sizeof(hilink_token)) == 0) {

		hi_at_printf("+SYSTOKEN:");

		for (unsigned int i = 0; i < sizeof(hilink_token); i++) {
			hi_at_printf("%c", hilink_token[i]);/* printf token to user check it */
		}
	} else {
		return HI_ERR_FAILURE ;
	}

	hi_at_printf("\r\nOK\r\n");

	return HI_ERR_SUCCESS;
}

hi_u32 cmd_set_systoken(hi_s32 argc, const hi_char *argv[]) {

	extern int HalWriteToken(const char *token, unsigned int len);

	char hilink_token[HILINK_TOKEN_LEN] = { 0 };

	if (argc != 4) { /* Token format:  ,,, */
		return HI_ERR_FAILURE ;
	}
	if (at_param_null_check(argc, argv) == HI_ERR_FAILURE) { /* check null */
		return HI_ERR_FAILURE ;
	}
	if (strlen(argv[0]) + strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3])
			!= (HILINK_TOKEN_LEN - 3)) { /* Token Len check 151-3,,, */
		return HI_ERR_FAILURE ;
	}

	sprintf(hilink_token, "%s,%s,%s,%s", argv[0], argv[1], argv[2], argv[3]);

	if (HalWriteToken(hilink_token, sizeof(hilink_token)) != 0) { /* write token */
		return HI_ERR_FAILURE ;
	}

	hi_at_printf("\r\nOK\r\n");

	return HI_ERR_SUCCESS;
}

hi_u32 at_exe_rfactory_cmd(void) {
	hi_at_printf("OK\r\n");
	HILINK_RestoreFactorySettings();
	HILINK_Reset();

	return HI_ERR_SUCCESS;
}
#endif

