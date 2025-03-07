/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * Copyright (c) 2021, Microsoft Corporation.
 *
 * Authors:
 *   Beau Belgrave <beaub@linux.microsoft.com>
 */
#ifndef _UAPI_LINUX_USER_EVENTS_H
#define _UAPI_LINUX_USER_EVENTS_H

#include </home/runner/EulerBot/linux/types.h>
#include </home/runner/EulerBot/linux/ioctl.h>

#ifdef __KERNEL__
#include </home/runner/EulerBot/linux/uio.h>
#else
#include <sys/uio.h>
#endif

#define USER_EVENTS_SYSTEM "user_events"
#define USER_EVENTS_PREFIX "u:"

/* Create dynamic location entry within a 32-bit value */
#define DYN_LOC(offset, size) ((size) << 16 | (offset))

/*
 * Describes an event registration and stores the results of the registration.
 * This structure is passed to the DIAG_IOCSREG ioctl, callers at a minimum
 * must set the size and name_args before invocation.
 */
struct user_reg {

	/* Input: Size of the user_reg structure being used */
	__u32 size;

	/* Input: Pointer to string with event name, description and flags */
	__u64 name_args;

	/* Output: Bitwise index of the event within the status page */
	__u32 status_bit;

	/* Output: Index of the event to use when writing data */
	__u32 write_index;
} __attribute__((__packed__));

#define DIAG_IOC_MAGIC '*'

/* Requests to register a user_event */
#define DIAG_IOCSREG _IOWR(DIAG_IOC_MAGIC, 0, struct user_reg*)

/* Requests to delete a user_event */
#define DIAG_IOCSDEL _IOW(DIAG_IOC_MAGIC, 1, char*)

#endif /* _UAPI_LINUX_USER_EVENTS_H */
