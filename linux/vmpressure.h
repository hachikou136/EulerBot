/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_VMPRESSURE_H
#define __LINUX_VMPRESSURE_H

#include </home/runner/EulerBot/linux/mutex.h>
#include </home/runner/EulerBot/linux/list.h>
#include </home/runner/EulerBot/linux/workqueue.h>
#include </home/runner/EulerBot/linux/gfp.h>
#include </home/runner/EulerBot/linux/types.h>
#include </home/runner/EulerBot/linux/cgroup.h>
#include </home/runner/EulerBot/linux/eventfd.h>

struct vmpressure {
	unsigned long scanned;
	unsigned long reclaimed;

	unsigned long tree_scanned;
	unsigned long tree_reclaimed;
	/* The lock is used to keep the scanned/reclaimed above in sync. */
	spinlock_t sr_lock;

	/* The list of vmpressure_event structs. */
	struct list_head events;
	/* Have to grab the lock on events traversal or modifications. */
	struct mutex events_lock;

	struct work_struct work;
};

struct mem_cgroup;

#ifdef CONFIG_MEMCG
extern void vmpressure(gfp_t gfp, struct mem_cgroup *memcg, bool tree,
		       unsigned long scanned, unsigned long reclaimed);
extern void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg, int prio);

extern void vmpressure_init(struct vmpressure *vmpr);
extern void vmpressure_cleanup(struct vmpressure *vmpr);
extern struct vmpressure *memcg_to_vmpressure(struct mem_cgroup *memcg);
extern struct mem_cgroup *vmpressure_to_memcg(struct vmpressure *vmpr);
extern int vmpressure_register_event(struct mem_cgroup *memcg,
				     struct eventfd_ctx *eventfd,
				     const char *args);
extern void vmpressure_unregister_event(struct mem_cgroup *memcg,
					struct eventfd_ctx *eventfd);
#else
static inline void vmpressure(gfp_t gfp, struct mem_cgroup *memcg, bool tree,
			      unsigned long scanned, unsigned long reclaimed) {}
static inline void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg,
				   int prio) {}
#endif /* CONFIG_MEMCG */
#endif /* __LINUX_VMPRESSURE_H */
