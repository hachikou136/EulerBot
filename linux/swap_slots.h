/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SWAP_SLOTS_H
#define _LINUX_SWAP_SLOTS_H

#include </home/runner/EulerBot/linux/swap.h>
#include </home/runner/EulerBot/linux/spinlock.h>
#include </home/runner/EulerBot/linux/mutex.h>

#define SWAP_SLOTS_CACHE_SIZE			SWAP_BATCH
#define THRESHOLD_ACTIVATE_SWAP_SLOTS_CACHE	(5*SWAP_SLOTS_CACHE_SIZE)
#define THRESHOLD_DEACTIVATE_SWAP_SLOTS_CACHE	(2*SWAP_SLOTS_CACHE_SIZE)

struct swap_slots_cache {
	bool		lock_initialized;
	struct mutex	alloc_lock; /* protects slots, nr, cur */
	swp_entry_t	*slots;
	int		nr;
	int		cur;
	spinlock_t	free_lock;  /* protects slots_ret, n_ret */
	swp_entry_t	*slots_ret;
	int		n_ret;
};

void disable_swap_slots_cache_lock(void);
void reenable_swap_slots_cache_unlock(void);
void enable_swap_slots_cache(void);
void free_swap_slot(swp_entry_t entry);

extern bool swap_slot_cache_enabled;

#endif /* _LINUX_SWAP_SLOTS_H */
