#ifndef _LIMITS_H
#define _LIMITS_H

#include <features.h>

#include <bits/alltypes.h> /* __LONG_MAX */
#ifdef __LITEOS__
#include "los_config.h"
#include "los_sem_pri.h"
#endif

/* Support signed or unsigned plain-char */

#if '\xff' > 0
#define CHAR_MIN 0
#define CHAR_MAX 255
#else
#define CHAR_MIN (-128)
#define CHAR_MAX 127
#endif

#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255
#define SHRT_MIN  (-1-0x7fff)
#define SHRT_MAX  0x7fff
#define USHRT_MAX 0xffff
#define INT_MIN  (-1-0x7fffffff)
#define INT_MAX  0x7fffffff
#define UINT_MAX 0xffffffffU
#define LONG_MIN (-LONG_MAX-1)
#define LONG_MAX __LONG_MAX
#define ULONG_MAX (2UL*LONG_MAX+1)
#define LLONG_MIN (-LLONG_MAX-1)
#define LLONG_MAX  0x7fffffffffffffffLL
#define ULLONG_MAX (2ULL*LLONG_MAX+1)

#define MB_LEN_MAX 4

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

#include <bits/limits.h>

#ifndef __LITEOS__
#define PIPE_BUF 4096
#else
#define PIPE_BUF 512
#endif
#define FILESIZEBITS 64
#ifndef NAME_MAX
#define NAME_MAX 255
#endif
#ifndef __LITEOS__
#define PATH_MAX 4096
#else
#define PATH_MAX 260
#define CONFIG_NAME_MAX NAME_MAX
#define CONFIG_PATH_MAX PATH_MAX
#endif

#ifdef __LITEOS__
#define NGROUPS_MAX 0
#define ARG_MAX 4096
#else
#define NGROUPS_MAX 32
#define ARG_MAX 131072
#endif
#define IOV_MAX 1024
#define SYMLOOP_MAX 40
#define WORD_BIT 32
#define SSIZE_MAX LONG_MAX
#define TZNAME_MAX 6
#define TTY_NAME_MAX 32
#define HOST_NAME_MAX 255

#if LONG_MAX == 0x7fffffffL
#define LONG_BIT 32
#else
#define LONG_BIT 64
#endif

/* Implementation choices... */

#define PTHREAD_KEYS_MAX 128
#ifndef __LITEOS__
#define PTHREAD_STACK_MIN 2048
#else
#define PTHREAD_STACK_MIN LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE
#endif
#define PTHREAD_DESTRUCTOR_ITERATIONS 4
#ifndef __LITEOS__
#define SEM_VALUE_MAX 0x7fffffff
#define SEM_NSEMS_MAX 256
#else
/**
 * @ingroup semaphore
 * Maximum semaphore value.
 */
#define SEM_VALUE_MAX OS_SEM_COUNT_MAX
/**
 * @ingroup semaphore
 * Maximum number of supported semaphores.
 */
#define SEM_NSEMS_MAX LOSCFG_BASE_IPC_SEM_LIMIT
#define OS_SEM_COUNT_MAX SEM_NSEMS_MAX
#endif
#define DELAYTIMER_MAX 0x7fffffff
#ifdef __LITEOS__
#define MQ_PRIO_MAX 1
#else
#define MQ_PRIO_MAX 32768
#endif
#define LOGIN_NAME_MAX 256

/* Arbitrary numbers... */

#define BC_BASE_MAX 99
#define BC_DIM_MAX 2048
#define BC_SCALE_MAX 99
#define BC_STRING_MAX 1000
#define CHARCLASS_NAME_MAX 14
#define COLL_WEIGHTS_MAX 2
#define EXPR_NEST_MAX 32
#define LINE_MAX 4096
#define RE_DUP_MAX 255

#define NL_ARGMAX 9
#define NL_MSGMAX 32767
#define NL_SETMAX 255
#define NL_TEXTMAX 2048

#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE)

#ifdef PAGESIZE
#define PAGE_SIZE PAGESIZE
#endif
#define NZERO 20
#define NL_LANGMAX 32

#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) \
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE+0 < 700)

#define NL_NMAX 16

#endif

/* POSIX/SUS requirements follow. These numbers come directly
 * from SUS and have nothing to do with the host system. */

#define _POSIX_AIO_LISTIO_MAX   2
#define _POSIX_AIO_MAX          1
#define _POSIX_ARG_MAX          4096
#define _POSIX_CHILD_MAX        25
#define _POSIX_CLOCKRES_MIN     20000000
#define _POSIX_DELAYTIMER_MAX   32
#define _POSIX_HOST_NAME_MAX    255
#define _POSIX_LINK_MAX         8
#define _POSIX_LOGIN_NAME_MAX   9
#define _POSIX_MAX_CANON        255
#define _POSIX_MAX_INPUT        255
#define _POSIX_MQ_OPEN_MAX      8
#define _POSIX_MQ_PRIO_MAX      32
#define _POSIX_NAME_MAX         14
#define _POSIX_NGROUPS_MAX      8
#define _POSIX_OPEN_MAX         20
#define _POSIX_PATH_MAX         256
#define _POSIX_PIPE_BUF         512
#define _POSIX_RE_DUP_MAX       255
#define _POSIX_RTSIG_MAX        8
#define _POSIX_SEM_NSEMS_MAX    256
#define _POSIX_SEM_VALUE_MAX    32767
#define _POSIX_SIGQUEUE_MAX     32
#define _POSIX_SSIZE_MAX        32767
#define _POSIX_STREAM_MAX       8
#define _POSIX_SS_REPL_MAX      4
#define _POSIX_SYMLINK_MAX      255
#define _POSIX_SYMLOOP_MAX      8
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4
#define _POSIX_THREAD_KEYS_MAX  128
#define _POSIX_THREAD_THREADS_MAX 64
#define _POSIX_TIMER_MAX        32
#define _POSIX_TRACE_EVENT_NAME_MAX 30
#define _POSIX_TRACE_NAME_MAX   8
#define _POSIX_TRACE_SYS_MAX    8
#define _POSIX_TRACE_USER_EVENT_MAX 32
#define _POSIX_TTY_NAME_MAX     9
#define _POSIX_TZNAME_MAX       6
#define _POSIX2_BC_BASE_MAX     99
#define _POSIX2_BC_DIM_MAX      2048
#define _POSIX2_BC_SCALE_MAX    99
#define _POSIX2_BC_STRING_MAX   1000
#define _POSIX2_CHARCLASS_NAME_MAX 14
#define _POSIX2_COLL_WEIGHTS_MAX 2
#define _POSIX2_EXPR_NEST_MAX   32
#define _POSIX2_LINE_MAX        2048
#define _POSIX2_RE_DUP_MAX      255

#define _XOPEN_IOV_MAX          16
#define _XOPEN_NAME_MAX         255
#define _XOPEN_PATH_MAX         1024

#ifdef __LITEOS__
#include "los_config.h"
#ifdef LOSCFG_FS_VFS
#include "vfs_config.h"
#endif

#define CHILD_MAX 6
#define _POSIX_CLK_TICK time((time_t *)NULL)
#define _POSIX_GETGR_R_SIZE_MAX 0
#define _POSIX_GETPW_R_SIZE_MAX 0
#define _LITEOS_MQ_OPEN_MAX LOSCFG_BASE_IPC_QUEUE_LIMIT
#define GETGR_R_SIZE_MAX _POSIX_GETGR_R_SIZE_MAX
#define GETPW_R_SIZE_MAX _POSIX_GETPW_R_SIZE_MAX

#ifdef LOSCFG_FS_VFS
#define OPEN_MAX CONFIG_NFILE_DESCRIPTORS
#else
#define OPEN_MAX 255
#endif
#define RTSIG_MAX 8
#define PTHREAD_ATTR_STACKSIZE LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE
#define PTHREAD_PRIORITY_SCHEDULING SCHED_OTHER
#define PTHREAD_THREADS_MAX (LOSCFG_BASE_CORE_TSK_LIMIT + 1)
#define TIMERS LOSCFG_BASE_CORE_SWTMR
#define TIMER_MAX _POSIX_TIMER_MAX
#define _POSIX_VERSION 200809L
#define POSIX_VERSION _POSIX_VERSION
#define SIGQUEUE_MAX 32
#define STREAM_MAX 8
#define MQ_OPEN_MAX _LITEOS_MQ_OPEN_MAX
#define SYS_CLK_TCK _POSIX_CLK_TICK
#define CONFIG_PATH_MAX PATH_MAX
#define LINK_MAX 8
#endif

#endif
