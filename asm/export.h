/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __ASM_GENERIC_EXPORT_H
#define __ASM_GENERIC_EXPORT_H

/*
 * This comment block is used by fixdep. Please do not remove.
 *
 * When CONFIG_MODVERSIONS is changed from n to y, all source files having
 * EXPORT_SYMBOL variants must be re-compiled because genksyms is run as a
 * side effect of the *.o build rule.
 */

#ifndef KSYM_FUNC
#define KSYM_FUNC(x) x
#endif
#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
#define KSYM_ALIGN 4
#elif defined(CONFIG_64BIT)
#define KSYM_ALIGN 8
#else
#define KSYM_ALIGN 4
#endif

.macro __put, val, name
#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
	.long	\val - ., \name - ., 0
#elif defined(CONFIG_64BIT)
	.quad	\val, \name, 0
#else
	.long	\val, \name, 0
#endif
.endm

/*
 * note on .section use: we specify progbits since usage of the "M" (SHF_MERGE)
 * section flag requires it. Use '%progbits' instead of '@progbits' since the
 * former apparently works on all arches according to the binutils source.
 */

.macro ___EXPORT_SYMBOL name,val,sec
#if defined(CONFIG_MODULES) && !defined(__DISABLE_EXPORTS)
	.section ___ksymtab\sec+\name,"a"
	.balign KSYM_ALIGN
__ksymtab_\name:
	__put \val, __kstrtab_\name
	.previous
	.section __ksymtab_strings,"aMS",%progbits,1
__kstrtab_\name:
	.asciz "\name"
	.previous
#endif
.endm

#if defined(CONFIG_TRIM_UNUSED_KSYMS)

#include </home/runner/EulerBot/linux/kconfig.h>
#include <generated/autoksyms.h>

.macro __ksym_marker sym
	.section ".discard.ksym","a"
__ksym_marker_\sym:
	 .previous
.endm

#define __EXPORT_SYMBOL(sym, val, sec)				\
	__ksym_marker sym;					\
	__cond_export_sym(sym, val, sec, __is_defined(__KSYM_##sym))
#define __cond_export_sym(sym, val, sec, conf)			\
	___cond_export_sym(sym, val, sec, conf)
#define ___cond_export_sym(sym, val, sec, enabled)		\
	__cond_export_sym_##enabled(sym, val, sec)
#define __cond_export_sym_1(sym, val, sec) ___EXPORT_SYMBOL sym, val, sec
#define __cond_export_sym_0(sym, val, sec) /* nothing */

#else
#define __EXPORT_SYMBOL(sym, val, sec) ___EXPORT_SYMBOL sym, val, sec
#endif

#define EXPORT_SYMBOL(name)					\
	__EXPORT_SYMBOL(name, KSYM_FUNC(name),)
#define EXPORT_SYMBOL_GPL(name) 				\
	__EXPORT_SYMBOL(name, KSYM_FUNC(name), _gpl)
#define EXPORT_DATA_SYMBOL(name)				\
	__EXPORT_SYMBOL(name, name,)
#define EXPORT_DATA_SYMBOL_GPL(name)				\
	__EXPORT_SYMBOL(name, name,_gpl)

#endif
