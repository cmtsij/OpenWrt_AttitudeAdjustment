/*
 *  Buffalo WCR-GN board support
 *
 *  Copyright (C) 2012 Frederic Leroy <fredo@starox.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

#include <asm/mach-ralink/machine.h>
#include <asm/mach-ralink/dev-gpio-buttons.h>
#include <asm/mach-ralink/dev-gpio-leds.h>
#include <asm/mach-ralink/rt305x.h>
#include <asm/mach-ralink/rt305x_regs.h>

#include "devices.h"

#define WCR_GN_GPIO_BUTTON_RESET	10
#define WCR_GN_GPIO_BUTTON_WPS	0

#define WCR_GN_GPIO_LED_DIAG_RED		7
#define WCR_GN_GPIO_LED_ROUTE_GREEN		11
#define WCR_GN_GPIO_LED_SECURITY_AMBER	13

#define WCR_GN_KEYS_POLL_INTERVAL	20
#define WCR_GN_KEYS_DEBOUNCE_INTERVAL (3 * WCR_GN_KEYS_POLL_INTERVAL)

const struct flash_platform_data wcr_gn_flash = {
	.type		= "mx25l3205d",
};

struct spi_board_info wcr_gn_spi_slave_info[] __initdata = {
	{
		.modalias	= "m25p80",
		.platform_data	= &wcr_gn_flash,
		.irq		= -1,
		.max_speed_hz	= 10000000,
		.bus_num	= 0,
		.chip_select	= 0,
	},
};

static struct gpio_keys_button wcr_gn_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = WCR_GN_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WCR_GN_GPIO_BUTTON_RESET,
		.active_low	= 1,
	},
	{
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = WCR_GN_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WCR_GN_GPIO_BUTTON_WPS,
		.active_low	= 1,
	}
};

static struct gpio_led wcr_gn_leds_gpio[] __initdata = {
	{
		.name		= "wcr-gn:red:diag",
		.gpio		= WCR_GN_GPIO_LED_DIAG_RED,
		.active_low	= 1,
	}, {
		.name		= "wcr-gn:green:router",
		.gpio		= WCR_GN_GPIO_LED_ROUTE_GREEN,
		.active_low	= 1,
	}, {
		.name		= "wcr-gn:security:amber",
		.gpio		= WCR_GN_GPIO_LED_SECURITY_AMBER,
		.active_low	= 1,
	}
};

static void __init wcr_gn_init(void)
{
	rt305x_gpio_init(RT305X_GPIO_MODE_GPIO << RT305X_GPIO_MODE_UART0_SHIFT);

	rt305x_register_spi(wcr_gn_spi_slave_info,
			    ARRAY_SIZE(wcr_gn_spi_slave_info));
	rt305x_esw_data.vlan_config = RT305X_ESW_VLAN_CONFIG_WLLLL;
	rt305x_register_ethernet();
	ramips_register_gpio_leds(-1, ARRAY_SIZE(wcr_gn_leds_gpio),
				  wcr_gn_leds_gpio);
	ramips_register_gpio_buttons(-1, WCR_GN_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(wcr_gn_gpio_buttons),
				     wcr_gn_gpio_buttons);
	rt305x_register_wifi();
	rt305x_register_usb();
	rt305x_register_wdt();
}

MIPS_MACHINE(RAMIPS_MACH_WCR_GN, "WCR-GN", "Buffalo WCR-GN",
	     wcr_gn_init);
