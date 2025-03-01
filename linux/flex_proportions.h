/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Floating proportions with flexible aging period
 *
 *  Copyright (C) 2011, SUSE, Jan Kara <jack@suse.cz>
 */

#ifndef _LINUX_FLEX_PROPORTIONS_H
#define _LINUX_FLEX_PROPORTIONS_H

#include </home/runner/EulerBot/linux/percpu_counter.h>
#include </home/runner/EulerBot/linux/spinlock.h>
#include </home/runner/EulerBot/linux/seqlock.h>
#include </home/runner/EulerBot/linux/gfp.h>

/*
 * When maximum proportion of some event type is specified, this is the
 * precision with which we allow limitting. Note that this creates an upper
 * bound on the number of events per period like
 *   ULLONG_MAX >> FPROP_FRAC_SHIFT.
 */
#define FPROP_FRAC_SHIFT 10
#define FPROP_FRAC_BASE (1UL << FPROP_FRAC_SHIFT)

/*
 * ---- Global proportion definitions ----
 */
struct fprop_global {
	/* Number of events in the current period */
	struct percpu_counter events;
	/* Current period */
	unsigned int period;
	/* Synchronization with period transitions */
	seqcount_t sequence;
};

int fprop_global_init(struct fprop_global *p, gfp_t gfp);
void fprop_global_destroy(struct fprop_global *p);
bool fprop_new_period(struct fprop_global *p, int periods);

/*
 *  ---- SINGLE ----
 */
struct fprop_local_single {
	/* the local events counter */
	unsigned long events;
	/* Period in which we last updated events */
	unsigned int period;
	raw_spinlock_t lock;	/* Protect period and numerator */
};

#define INIT_FPROP_LOCAL_SINGLE(name)			\
{	.lock = __RAW_SPIN_LOCK_UNLOCKED(name.lock),	\
}

int fprop_local_init_single(struct fprop_local_single *pl);
void fprop_local_destroy_single(struct fprop_local_single *pl);
void __fprop_inc_single(struct fprop_global *p, struct fprop_local_single *pl);
void fprop_fraction_single(struct fprop_global *p,
	struct fprop_local_single *pl, unsigned long *numerator,
	unsigned long *denominator);

static inline
void fprop_inc_single(struct fprop_global *p, struct fprop_local_single *pl)
{
	unsigned long flags;

	local_irq_save(flags);
	__fprop_inc_single(p, pl);
	local_irq_restore(flags);
}

/*
 * ---- PERCPU ----
 */
struct fprop_local_percpu {
	/* the local events counter */
	struct percpu_counter events;
	/* Period in which we last updated events */
	unsigned int period;
	raw_spinlock_t lock;	/* Protect period and numerator */
};

int fprop_local_init_percpu(struct fprop_local_percpu *pl, gfp_t gfp);
void fprop_local_destroy_percpu(struct fprop_local_percpu *pl);
void __fprop_add_percpu(struct fprop_global *p, struct fprop_local_percpu *pl,
		long nr);
void __fprop_add_percpu_max(struct fprop_global *p,
		struct fprop_local_percpu *pl, int max_frac, long nr);
void fprop_fraction_percpu(struct fprop_global *p,
	struct fprop_local_percpu *pl, unsigned long *numerator,
	unsigned long *denominator);

static inline
void fprop_inc_percpu(struct fprop_global *p, struct fprop_local_percpu *pl)
{
	unsigned long flags;

	local_irq_save(flags);
	__fprop_add_percpu(p, pl, 1);
	local_irq_restore(flags);
}

#endif
