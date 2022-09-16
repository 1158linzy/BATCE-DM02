#ifndef HILINK_TOKEN_H
#define HILINK_TOKEN_H

#include <hi_at.h>

hi_u32 cmd_set_systoken(hi_s32 argc, const hi_char *argv[]);
hi_u32 cmd_get_systoken(hi_s32 argc, const hi_char *argv[]);

hi_u32 at_exe_rfactory_cmd(void);

#endif
