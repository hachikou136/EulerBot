/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_NS_H
#define _LINUX_PID_NS_H

#include </home/runner/EulerBot/linux/sched.h>
#include </home/runner/EulerBot/linux/bug.h>
#include </home/runner/EulerBot/linux/mm.h>
#include </home/runner/EulerBot/linux/workqueue.h>
#include </home/runner/EulerBot/linux/threads.h>
#include </home/runner/EulerBot/linux/nsproxy.h>
#include </home/runner/EulerBot/linux/ns_common.h>
#include </home/runner/EulerBot/linux/idr.h>

/* MAX_PID_NS_LEVEL is needed for limiting size of 'struct pid' */
#define MAX_PID_NS_LEVEL 32

struct fs_pin;

#if defined(CONFIG_SYSCTL) && defined(CONFIG_MEMFD_CREATE)
/*
 * sysctl for vm.memfd_noexec
 * 0: memfd_create() without MFD_EXEC nor MFD_NOEXEC_SEAL
 *	acts like MFD_EXEC was set.
 * 1: memfd_create() without MFD_EXEC nor MFD_NOEXEC_SEAL
 *	acts like MFD_NOEXEC_SEAL was set.
 * 2: memfd_create() without MFD_NOEXEC_SEAL will be
 *	rejected.
 */
#define MEMFD_NOEXEC_SCOPE_EXEC			0
#define MEMFD_NOEXEC_SCOPE_NOEXEC_SEAL		1
#define MEMFD_NOEXEC_SCOPE_NOEXEC_ENFORCED	2
#endif

struct pid_namespace {
	struct idr idr;
	struct rcu_head rcu;
	unsigned int pid_allocated;
	struct task_struct *child_reaper;
	struct kmem_cache *pid_cachep;
	unsigned int level;
	struct pid_namespace *parent;
#ifdef CONFIG_BSD_PROCESS_ACCT
	struct fs_pin *bacct;
#endif
	struct user_namespace *user_ns;
	struct ucounts *ucounts;
	int reboot;	/* group exit code if this pidns was rebooted */
	struct ns_common ns;
#if defined(CONFIG_SYSCTL) && defined(CONFIG_MEMFD_CREATE)
	/* sysctl for vm.memfd_noexec */
	int memfd_noexec_scope;
#endif
} __randomize_layout;

extern struct pid_namespace init_pid_ns;

#define PIDNS_ADDING (1U << 31)

#ifdef CONFIG_PID_NS
static inline struct pid_namespace *get_pid_ns(struct pid_namespace *ns)
{
	if (ns != &init_pid_ns)
		refcount_inc(&ns->ns.count);
	return ns;
}

extern struct pid_namespace *copy_pid_ns(unsigned long flags,
	struct user_namespace *user_ns, struct pid_namespace *ns);
extern void zap_pid_ns_processes(struct pid_namespace *pid_ns);
extern int reboot_pid_ns(struct pid_namespace *pid_ns, int cmd);
extern void put_pid_ns(struct pid_namespace *ns);

#else /* !CONFIG_PID_NS */
#include </home/runner/EulerBot/linux/err.h>

static inline struct pid_namespace *get_pid_ns(struct pid_namespace *ns)
{
	return ns;
}

static inline struct pid_namespace *copy_pid_ns(unsigned long flags,
	struct user_namespace *user_ns, struct pid_namespace *ns)
{
	if (flags & CLONE_NEWPID)
		ns = ERR_PTR(-EINVAL);
	return ns;
}

static inline void put_pid_ns(struct pid_namespace *ns)
{
}

static inline void zap_pid_ns_processes(struct pid_namespace *ns)
{
	BUG();
}

static inline int reboot_pid_ns(struct pid_namespace *pid_ns, int cmd)
{
	return 0;
}
#endif /* CONFIG_PID_NS */

extern struct pid_namespace *task_active_pid_ns(struct task_struct *tsk);
void pidhash_init(void);
void pid_idr_init(void);

static inline bool task_is_in_init_pid_ns(struct task_struct *tsk)
{
	return task_active_pid_ns(tsk) == &init_pid_ns;
}

#endif /* _LINUX_PID_NS_H */
