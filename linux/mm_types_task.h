/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MM_TYPES_TASK_H
#define _LINUX_MM_TYPES_TASK_H

/*
 * Here are the definitions of the MM data types that are embedded in 'struct task_struct'.
 *
 * (These are defined separately to decouple sched.h from mm_types.h as much as possible.)
 */

#include </home/runner/EulerBot/linux/types.h>
#include </home/runner/EulerBot/linux/threads.h>
#include </home/runner/EulerBot/linux/atomic.h>
#include </home/runner/EulerBot/linux/cpumask.h>

#include </home/runner/EulerBot/asm/page.h>

#ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
#include </home/runner/EulerBot/asm/tlbbatch.h>
#endif

#define USE_SPLIT_PTE_PTLOCKS	(NR_CPUS >= CONFIG_SPLIT_PTLOCK_CPUS)
#define USE_SPLIT_PMD_PTLOCKS	(USE_SPLIT_PTE_PTLOCKS && \
		IS_ENABLED(CONFIG_ARCH_ENABLE_SPLIT_PMD_PTLOCK))
#define ALLOC_SPLIT_PTLOCKS	(SPINLOCK_SIZE > BITS_PER_LONG/8)

/*
 * When updating this, please also update struct resident_page_types[] in
 * kernel/fork.c
 */
enum {
	MM_FILEPAGES,	/* Resident file mapping pages */
	MM_ANONPAGES,	/* Resident anonymous pages */
	MM_SWAPENTS,	/* Anonymous swap entries */
	MM_SHMEMPAGES,	/* Resident shared memory pages */
	NR_MM_COUNTERS
};

struct page_frag {
	struct page *page;
#if (BITS_PER_LONG > 32) || (PAGE_SIZE >= 65536)
	__u32 offset;
	__u32 size;
#else
	__u16 offset;
	__u16 size;
#endif
};

/* Track pages that require TLB flushes */
struct tlbflush_unmap_batch {
#ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
	/*
	 * The arch code makes the following promise: generic code can modify a
	 * PTE, then call arch_tlbbatch_add_mm() (which internally provides all
	 * needed barriers), then call arch_tlbbatch_flush(), and the entries
	 * will be flushed on all CPUs by the time that arch_tlbbatch_flush()
	 * returns.
	 */
	struct arch_tlbflush_unmap_batch arch;

	/* True if a flush is needed. */
	bool flush_required;

	/*
	 * If true then the PTE was dirty when unmapped. The entry must be
	 * flushed before IO is initiated or a stale TLB entry potentially
	 * allows an update without redirtying the page.
	 */
	bool writable;
#endif
};

#endif /* _LINUX_MM_TYPES_TASK_H */
