#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/opt/fsl-imx-wayland/4.14-sumo/sysroots/aarch64-poky-linux
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/opt/fsl-imx-wayland/4.14-sumo/sysroots/aarch64-poky-linux/usr/lib/pkgconfig:/opt/fsl-imx-wayland/4.14-sumo/sysroots/aarch64-poky-linux/usr/share/pkgconfig:/opt/fsl-imx-wayland/4.14-sumo/sysroots/aarch64-poky-linux/usr/lib/aarch64-poky-linux/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
