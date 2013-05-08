/*
 *  Vizio XWR100 board support
 *
 *  Copyright (C) 2009-2012 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2009 Imre Kaloz <kaloz@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <asm/mach-ath79/ath79.h>

#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-usb.h"
#include "machtypes.h"

#define XWR100_GPIO_LED_USB  		0
#define XWR100_GPIO_LED_POWER		1
#define XWR100_GPIO_LED_WPS  		5
#define XWR100_GPIO_LED_WLAN 		6

#define XWR100_GPIO_BTN_RESET		3
#define XWR100_GPIO_BTN_WPS  		8

#define XWR100_KEYS_POLL_INTERVAL	20	/* msecs */
#define XWR100_KEYS_DEBOUNE_INTERVAL	(3 * XWR100_KEYS_POLL_INTERVAL)

#define XWR100_CALDATA0_OFFSET		0x1000
#define XWR100_CALDATA1_OFFSET		0x5000
#define XWR100_MAC0_ADDR_OFFSET		(XWR100_CALDATA0_OFFSET+0x10C)
#define XWR100_MAC1_ADDR_OFFSET		(XWR100_CALDATA1_OFFSET+0x20C)


static struct mtd_partition xwr100_partitions[] = {
	{
		.name		= "uboot",
		.offset		= 0,
		.size		= 0x030000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "env",
		.offset		= 0x030000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "linux",
		.offset		= 0x040000,
		.size		= 0x170000,
	}, {
		.name		= "rootfs",
		.offset		= 0x1b0000,
		.size		= 0x600000,
	}, {
		.name		= "nvram",
		.offset		= 0x7b0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "factory",
		.offset		= 0x7c0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "language",
		.offset		= 0x7d0000,
		.size		= 0x020000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "caldata",
		.offset		= 0x7f0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "firmware",
		.offset		= 0x040000,
		.size		= 0x770000,
	}
};

static struct flash_platform_data xwr100_flash_data = {
	.parts		= xwr100_partitions,
	.nr_parts	= ARRAY_SIZE(xwr100_partitions),
};

static struct gpio_led xwr100_leds_gpio[] __initdata = {
	{
		.name		= "xwr100:white:usb",
		.gpio		= XWR100_GPIO_LED_USB,
		.active_low	= 1,
	}, {
		.name		= "xwr100:white:power",
		.gpio		= XWR100_GPIO_LED_POWER,
		.active_low	= 0,
		.default_trigger = "default-on",
	}, {
		.name		= "xwr100:white:wps",
		.gpio		= XWR100_GPIO_LED_WPS,
		.active_low	= 1,
	}, {
		.name		= "xwr100:white:wlan",
		.gpio		= XWR100_GPIO_LED_WLAN,
		.active_low	= 1,
	}
};

static struct gpio_keys_button xwr100_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = XWR100_KEYS_DEBOUNE_INTERVAL,
		.gpio		= XWR100_GPIO_BTN_RESET,
		.active_low	= 1,
	}, {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = XWR100_KEYS_DEBOUNE_INTERVAL,
		.gpio		= XWR100_GPIO_BTN_WPS,
		.active_low	= 1,
	}
};

static void __init xwr100_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);
	u8 *mac = art + XWR100_MAC0_ADDR_OFFSET;

	ath79_register_mdio(0, 0x0);

	ath79_init_mac(ath79_eth1_data.mac_addr, mac, 2);
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_RMII;
	ath79_eth1_data.speed = SPEED_100;
	ath79_eth1_data.duplex = DUPLEX_FULL;

	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 1);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RMII;
	ath79_eth0_data.phy_mask = 0x10;

	ath79_register_eth(0);
	ath79_register_eth(1);
	
	ath79_register_usb();

	ath79_register_m25p80(&xwr100_flash_data);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(xwr100_leds_gpio),
				 xwr100_leds_gpio);

	ath79_register_gpio_keys_polled(-1, XWR100_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(xwr100_gpio_keys),
					xwr100_gpio_keys);

	ap94_pci_init(art + XWR100_CALDATA0_OFFSET, 
			art + XWR100_MAC0_ADDR_OFFSET,
			art + XWR100_CALDATA1_OFFSET, 
			art + XWR100_MAC1_ADDR_OFFSET);
}

MIPS_MACHINE(ATH79_MACH_XWR100, "XWR100", "Vizio XWR100", xwr100_setup);
