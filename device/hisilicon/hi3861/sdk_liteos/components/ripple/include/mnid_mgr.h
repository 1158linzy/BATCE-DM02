/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: RPL Mesh Node ID Manager headers
 * Author: NA
 * Create: 2019-04-25
 */

#ifndef _MNID_MGR_H_
#define _MNID_MGR_H_

#include "ripple.h"

uint8_t rpl_mnode_id_mgr_init(rpl_mnode_id_t start, rpl_mnode_id_t end);
#if RPL_CONF_MMBR_MNID
uint8_t rpl_mnode_id_mgr_alloc(rpl_linkaddr_t *lladdr, rpl_mnode_id_t prfid, rpl_mnode_id_t *mnid);
void rpl_mnode_id_mgr_release(rpl_linkaddr_t *lladdr, rpl_mnode_id_t mnid);
#else
uint8_t rpl_mnode_id_mgr_alloc(rpl_mnode_id_t prfid, rpl_mnode_id_t *mnid);
void rpl_mnode_id_mgr_release(rpl_mnode_id_t mnid);
#endif

#endif /* _MNID_MGR_H_ */
