/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ERROR_INJECTION_H
#define _LINUX_ERROR_INJECTION_H

#include </home/runner/EulerBot/linux/compiler.h>
#include </home/runner/EulerBot/linux/errno.h>
#include <asm-generic/error-injection.h>

#ifdef CONFIG_FUNCTION_ERROR_INJECTION

extern bool within_error_injection_list(unsigned long addr);
extern int get_injectable_error_type(unsigned long addr);

#else /* !CONFIG_FUNCTION_ERROR_INJECTION */

static inline bool within_error_injection_list(unsigned long addr)
{
	return false;
}

static inline int get_injectable_error_type(unsigned long addr)
{
	return -EOPNOTSUPP;
}

#endif

#endif /* _LINUX_ERROR_INJECTION_H */
