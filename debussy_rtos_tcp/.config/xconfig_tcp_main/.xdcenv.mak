#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirtos_1_21_00_09/packages;C:/ti/tirtos_1_21_00_09/products/bios_6_37_00_20/packages;C:/ti/tirtos_1_21_00_09/products/ipc_3_10_01_11/packages;C:/ti/tirtos_1_21_00_09/products/ndk_2_23_01_01/packages;C:/ti/tirtos_1_21_00_09/products/uia_1_04_00_06/packages;C:/ti/ccsv5/ccs_base;C:/Users/Risto/workspace_v5_5/debussy_rtos_tcp/.config
override XDCROOT = C:/ti/xdctools_3_25_03_72
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirtos_1_21_00_09/packages;C:/ti/tirtos_1_21_00_09/products/bios_6_37_00_20/packages;C:/ti/tirtos_1_21_00_09/products/ipc_3_10_01_11/packages;C:/ti/tirtos_1_21_00_09/products/ndk_2_23_01_01/packages;C:/ti/tirtos_1_21_00_09/products/uia_1_04_00_06/packages;C:/ti/ccsv5/ccs_base;C:/Users/Risto/workspace_v5_5/debussy_rtos_tcp/.config;C:/ti/xdctools_3_25_03_72/packages;..
HOSTOS = Windows
endif
