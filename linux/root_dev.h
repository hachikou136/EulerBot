/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ROOT_DEV_H_
#define _ROOT_DEV_H_

#include </home/runner/EulerBot/linux/major.h>
#include </home/runner/EulerBot/linux/types.h>
#include </home/runner/EulerBot/linux/kdev_t.h>

enum {
	Root_NFS = MKDEV(UNNAMED_MAJOR, 255),
	Root_CIFS = MKDEV(UNNAMED_MAJOR, 254),
	Root_RAM0 = MKDEV(RAMDISK_MAJOR, 0),
	Root_RAM1 = MKDEV(RAMDISK_MAJOR, 1),
	Root_FD0 = MKDEV(FLOPPY_MAJOR, 0),
	Root_HDA1 = MKDEV(IDE0_MAJOR, 1),
	Root_HDA2 = MKDEV(IDE0_MAJOR, 2),
	Root_SDA1 = MKDEV(SCSI_DISK0_MAJOR, 1),
	Root_SDA2 = MKDEV(SCSI_DISK0_MAJOR, 2),
	Root_HDC1 = MKDEV(IDE1_MAJOR, 1),
	Root_SR0 = MKDEV(SCSI_CDROM_MAJOR, 0),
};

extern dev_t ROOT_DEV;

#endif
