/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef __LINUX_KMOD_H__
#define __LINUX_KMOD_H__

/*
 *	include/linux/kmod.h
 */

#include </home/runner/EulerBot/linux/umh.h>
#include </home/runner/EulerBot/linux/gfp.h>
#include </home/runner/EulerBot/linux/stddef.h>
#include </home/runner/EulerBot/linux/errno.h>
#include </home/runner/EulerBot/linux/compiler.h>
#include </home/runner/EulerBot/linux/workqueue.h>
#include </home/runner/EulerBot/linux/sysctl.h>

#define KMOD_PATH_LEN 256

#ifdef CONFIG_MODULES
extern char modprobe_path[]; /* for sysctl */
/* modprobe exit status on success, -ve on error.  Return value
 * usually useless though. */
extern __printf(2, 3)
int __request_module(bool wait, const char *name, ...);
#define request_module(mod...) __request_module(true, mod)
#define request_module_nowait(mod...) __request_module(false, mod)
#define try_then_request_module(x, mod...) \
	((x) ?: (__request_module(true, mod), (x)))
#else
static inline int request_module(const char *name, ...) { return -ENOSYS; }
static inline int request_module_nowait(const char *name, ...) { return -ENOSYS; }
#define try_then_request_module(x, mod...) (x)
#endif

#endif /* __LINUX_KMOD_H__ */
