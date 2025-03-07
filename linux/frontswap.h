/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FRONTSWAP_H
#define _LINUX_FRONTSWAP_H

#include </home/runner/EulerBot/linux/swap.h>
#include </home/runner/EulerBot/linux/mm.h>
#include </home/runner/EulerBot/linux/bitops.h>
#include </home/runner/EulerBot/linux/jump_label.h>

struct frontswap_ops {
	void (*init)(unsigned); /* this swap type was just swapon'ed */
	int (*store)(unsigned, pgoff_t, struct page *); /* store a page */
	int (*load)(unsigned, pgoff_t, struct page *); /* load a page */
	void (*invalidate_page)(unsigned, pgoff_t); /* page no longer needed */
	void (*invalidate_area)(unsigned); /* swap type just swapoff'ed */
};

int frontswap_register_ops(const struct frontswap_ops *ops);

extern void frontswap_init(unsigned type, unsigned long *map);
extern int __frontswap_store(struct page *page);
extern int __frontswap_load(struct page *page);
extern void __frontswap_invalidate_page(unsigned, pgoff_t);
extern void __frontswap_invalidate_area(unsigned);

#ifdef CONFIG_FRONTSWAP
extern struct static_key_false frontswap_enabled_key;

static inline bool frontswap_enabled(void)
{
	return static_branch_unlikely(&frontswap_enabled_key);
}

static inline void frontswap_map_set(struct swap_info_struct *p,
				     unsigned long *map)
{
	p->frontswap_map = map;
}

static inline unsigned long *frontswap_map_get(struct swap_info_struct *p)
{
	return p->frontswap_map;
}
#else
/* all inline routines become no-ops and all externs are ignored */

static inline bool frontswap_enabled(void)
{
	return false;
}

static inline void frontswap_map_set(struct swap_info_struct *p,
				     unsigned long *map)
{
}

static inline unsigned long *frontswap_map_get(struct swap_info_struct *p)
{
	return NULL;
}
#endif

static inline int frontswap_store(struct page *page)
{
	if (frontswap_enabled())
		return __frontswap_store(page);

	return -1;
}

static inline int frontswap_load(struct page *page)
{
	if (frontswap_enabled())
		return __frontswap_load(page);

	return -1;
}

static inline void frontswap_invalidate_page(unsigned type, pgoff_t offset)
{
	if (frontswap_enabled())
		__frontswap_invalidate_page(type, offset);
}

static inline void frontswap_invalidate_area(unsigned type)
{
	if (frontswap_enabled())
		__frontswap_invalidate_area(type);
}

#endif /* _LINUX_FRONTSWAP_H */
