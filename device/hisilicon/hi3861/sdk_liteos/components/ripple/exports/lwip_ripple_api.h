/**
 * @file lwip_ripple_api.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved. \n
 *
 * description: the api of the rpl will be called in the user thread.\n
 *
 * Author: NA
 *
 * Create: 2019
 */
/** @defgroup Threadsafe_Rpl_Interfaces rpl */

#ifndef _RPL_LWIP_RIPPLE_API_H_
#define _RPL_LWIP_RIPPLE_API_H_

#include <stdint.h>

#define RPL_MODE_MBR 1 /* Root mode */
#define RPL_MODE_MG 2  /* Router mode */

#define RPL_6ADDR_LEN 16
#define RPL_CONFIG_SIZE 16

#define RPL_MAC_ADDR_LEN 6
enum ROUTE_SYNC_TYPE {
  ROUTE_ADD = 100,
  ROUTE_DEL,
  ROUTE_NEXTHOP,
  ROUTE_LIFITIEM,
  ROUTE_LEN,
  ROUTE_MNID,
  ROUTE_PATH_SEQ,
  ROUTE_NON_MESH,
  ROUTE_ALL,
  ROUTE_GET_ALL_ENTRY, /* Get all route entry: include sync and self generate */
  ROUTE_GET_MBR_SELF_ENTRY, /* Get all mbr self generate route entry */
  ROUTE_GET_MBR_FOR_TOP /* Get MBR self generate route entry not modify nhop */
};

typedef struct {
  uint8_t target[RPL_6ADDR_LEN];
  uint8_t nexthop[RPL_6ADDR_LEN];
  uint16_t lifetime;
  uint8_t len;
  uint8_t mnid;
  uint8_t path_seq;
  uint8_t non_mesh;
  uint8_t route_sync;
  uint32_t conn_time;
} rpl_route_entry_t;

typedef struct {
  uint8_t resv[RPL_CONFIG_SIZE];
} rpl_config_t;

typedef struct {
  uint8_t prefix[RPL_6ADDR_LEN];
  uint8_t len;
} rpl_ip6_prefix_t;

typedef struct {
  uint8_t inited;
  uint8_t running;
  uint8_t mode;
  uint8_t instid;
  uint8_t isroot;
  uint8_t mnid;
  uint8_t rank;
  uint8_t state;
  uint8_t dodagid[RPL_6ADDR_LEN];
  uint8_t parent[RPL_6ADDR_LEN];
} rpl_info_t;

/*
 * Func Name: rpl_init
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to init the RPL module.
 *
 * @param[in]    name         Indicates the network interface name to work on.
 * @param[in]    len          Indicates the network interface name len.
 *
 * @returns
 *  >0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - there is only one rpl in the system. when this api is called many times in different netif,
 *     this api will return negative value.
 */
int rpl_init(const char *name, uint8_t len);

/*
 * Func Name: rpl_deinit
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to deinit the rpl.
 *
 * @param[in]    ctx          Indicates the rpl context to work on.
 *
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - unload the rpl module, this api will also do rpl stop job.
 */
int rpl_deinit(int ctx);

/*
 * Func Name:  rpl_start
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to start the RPL.
 *
 * @param[in]    ctx            Indicates the rpl context to work on.
 * @param[in]    mode           Indicates the role in the RPL(MBR or MG).
 * @param[in]    instID         Indicates the instance id of the rpl.
 * @param[in]    cfg            Indicates the config of the rpl, this arg is not used.
 * @param[in]    prefix         Indicates the prefix of the rpl.
 *
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - the rpl must be init before using this api.
 */
int rpl_start(int ctx, uint8_t mode, uint8_t inst_id,
              const rpl_config_t *cfg, const rpl_ip6_prefix_t *prefix);

/*
 * Func Name: rpl_stop
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to stop rpl.
 *
 * @param[in]    ctx            Indicates the rpl context to work on.
 *
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - just stop timer and free instance, but not free the netif.
 */
int rpl_stop(int ctx);

/*
 * Func Name:  rpl_route_entry_count
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to get the current route entry numbers.
 *
 * @param[in]    ctx         Indicates the rpl context to work on.
 * @param[out]   cnt         Indicates the route entry numbers.
 * @param[in]    type        Indicates the route entry type
 *
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - the numbers of the route entry may be changed after call this api.
 */
int rpl_route_entry_count(int ctx, uint16_t *cnt, enum ROUTE_SYNC_TYPE type);

/*
 * Func Name:  rpl_route_entry_get
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to get the route entry info.
 *
 * @param[in]    ctx            Indicates the rpl context to work on.
 * @param[out]   rte            Indicates the buf that the route entry info will be stored.
 * @param[out]   cnt            Indicates the number of entry will be copyed.
 * @param[in]    type           Indicates the operate type for route entry.
 *               ROUTE_GET_ALL_ENTRY Get all route entry:include sync and self generate
 *               ROUTE_GET_MBR_SELF_ENTRY Get all mbr self generate route entry
 * @returns
 *  >0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - if all the route entry are wanted, the cnt should be the maximum value.
 */
int rpl_route_entry_get(int ctx, rpl_route_entry_t *rte, uint16_t cnt, enum ROUTE_SYNC_TYPE type);

/*
 * Func Name:  rpl_info_get
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to get the rpl info.
 *
 * @param[out]   info            Indicates the info of the node in the rpl network.
 *
 * @returns
 *  =0 : On success \n
 *  Negative value : On failure \n
 *
 */
int rpl_info_get(rpl_info_t *info);

/*
 * Func Name:  rpl_rank_get
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to get the rpl rank info.
 *
 * @param[out]   rank            Indicates the rank of the node in the rpl network.
 *
 * @returns
 *  =0 : On success \n
 *  Negative value : On failure \n
 *
 */
int rpl_rank_get(uint16_t *rank);

/*
 * Func Name:  rpl_rank_threshold_set
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to set the rpl rank threshold.
 *
 * @param[out]   rank            Indicates the rank threshold of the node in the rpl network.
 *
 * @returns
 *  =0 : On success \n
 *  Negative value : On failure \n
 *
 */
int rpl_rank_threshold_set(uint16_t rank_threshold);

/*
 * Func Name: rpl_route_entry_delete
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 * This is a thread safe API, used to delete the route entry.
 *
 * @param[in]    ctx         Indicates the rpl context to work on.
 * @param[in]    rte         the route entry from api
 *
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - this api is used to delete the route entry from MBR which is sync entry
 */
int rpl_route_entry_delete(int ctx, rpl_route_entry_t *rte);

/*
 * Func Name: rpl_route_entry_add
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 * This is a thread safe API, used to add the route entry.
 *
 * @param[in]    ctx         Indicates the rpl context to work on.
 * @param[in]    rte         the route entry from api
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - this api is used to add the route entry to MBR
 */
int rpl_route_entry_add(int ctx, rpl_route_entry_t *rte);

/*
 * Func Name: rpl_route_entry_update
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 * This is a thread safe API, used to update the route entry
 *
 * @param[in]    ctx         Indicates the rpl context to work on.
 * @param[in]    rte         the route entry from api
 * @param[in]    type         the type to change
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - this api is used to update the route entry to MBR
 */
int rpl_route_entry_update(int ctx, rpl_route_entry_t *rte, enum ROUTE_SYNC_TYPE type);

/*
 * Func Name: rpl_sync_route_del_by_mnid
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 * This is a thread safe API, used to delete the route entry by mnid
 *
 * @param[in]    ctx         Indicates the rpl context to work on.
 * @param[in]    type        the mind from api
 * @returns
 *  0 : On success \n
 *  Negative value : On failure \n
 *
 * @note
 *   - this api is used to delete the route entry by mnid
 */
int rpl_sync_route_del_by_mnid(int ctx, uint8_t mnid);

/*
 * Func Name:  rpl_working_dag_dodagid_get
 */
/**
 * @ingroup Threadsafe_Rpl_Interfaces
 *
 * @brief
 *
 *  This is a thread safe API, used to get the rpl dodagid of the working dag.
 *
 * @param[out]   dodagid        Indicates the buffer of the dodagid;
 * @param[out]   len            Indicates the length of the buffer;
 *
 * @returns
 *  =0 : On success \n
 *  Negative value : On failure \n
 *
 */
int rpl_working_dag_dodagid_get(uint8_t *dodagid, uint8_t len);
#endif
