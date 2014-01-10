#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/XWR100
	NAME:=Vizio XWR100
	PACKAGES:=kmod-usb-core kmod-usb2 kmod-usb-ohci kmod-ledtrig-usbdev
endef

define Profile/XWR100/Description
	Package set optimized for the Vizio XWR100.
endef

$(eval $(call Profile,XWR100))
