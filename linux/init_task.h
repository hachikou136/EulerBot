/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX__INIT_TASK_H
#define _LINUX__INIT_TASK_H

#include </home/runner/EulerBot/linux/rcupdate.h>
#include </home/runner/EulerBot/linux/irqflags.h>
#include </home/runner/EulerBot/linux/utsname.h>
#include </home/runner/EulerBot/linux/lockdep.h>
#include </home/runner/EulerBot/linux/ftrace.h>
#include </home/runner/EulerBot/linux/ipc.h>
#include </home/runner/EulerBot/linux/pid_namespace.h>
#include </home/runner/EulerBot/linux/user_namespace.h>
#include </home/runner/EulerBot/linux/securebits.h>
#include </home/runner/EulerBot/linux/seqlock.h>
#include </home/runner/EulerBot/linux/rbtree.h>
#include </home/runner/EulerBot/linux/refcount.h>
#include </home/runner/EulerBot/linux/sched/autogroup.h>
#include <net/net_namespace.h>
#include </home/runner/EulerBot/linux/sched/rt.h>
#include </home/runner/EulerBot/linux/livepatch.h>
#include </home/runner/EulerBot/linux/mm_types.h>

#include <asm/thread_info.h>

extern struct files_struct init_files;
extern struct fs_struct init_fs;
extern struct nsproxy init_nsproxy;
extern struct cred init_cred;

#ifndef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
#define INIT_PREV_CPUTIME(x)	.prev_cputime = {			\
	.lock = __RAW_SPIN_LOCK_UNLOCKED(x.prev_cputime.lock),		\
},
#else
#define INIT_PREV_CPUTIME(x)
#endif

#define INIT_TASK_COMM "swapper"

/* Attach to the init_task data structure for proper alignment */
#ifdef CONFIG_ARCH_TASK_STRUCT_ON_STACK
#define __init_task_data __section(".data..init_task")
#else
#define __init_task_data /**/
#endif

/* Attach to the thread_info data structure for proper alignment */
#define __init_thread_info __section(".data..init_thread_info")

#endif
