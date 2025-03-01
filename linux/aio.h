/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX__AIO_H
#define __LINUX__AIO_H

#include </home/runner/EulerBot/linux/aio_abi.h>

struct kioctx;
struct kiocb;
struct mm_struct;

typedef int (kiocb_cancel_fn)(struct kiocb *);

/* prototypes */
#ifdef CONFIG_AIO
extern void exit_aio(struct mm_struct *mm);
void kiocb_set_cancel_fn(struct kiocb *req, kiocb_cancel_fn *cancel);
#else
static inline void exit_aio(struct mm_struct *mm) { }
static inline void kiocb_set_cancel_fn(struct kiocb *req,
				       kiocb_cancel_fn *cancel) { }
#endif /* CONFIG_AIO */

#endif /* __LINUX__AIO_H */
