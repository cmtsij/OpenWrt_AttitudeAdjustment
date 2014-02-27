#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/WCR_GN
	NAME:=Buffalo WCR-GN
	PACKAGES:=
endef

define Profile/WCR_GN/Description
	Package set for Buffalo WCR-GN board
endef

$(eval $(call Profile,WCR_GN))
