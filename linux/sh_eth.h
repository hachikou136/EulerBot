/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_ETH_H__
#define __ASM_SH_ETH_H__

#include </home/runner/EulerBot/linux/phy.h>
#include </home/runner/EulerBot/linux/if_ether.h>

struct sh_eth_plat_data {
	int phy;
	int phy_irq;
	phy_interface_t phy_interface;
	void (*set_mdio_gate)(void *addr);

	unsigned char mac_addr[ETH_ALEN];
	unsigned no_ether_link:1;
	unsigned ether_link_active_low:1;
};

#endif
