## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/tcp_main_pem3.oem3

linker.cmd: package/cfg/tcp_main_pem3.xdl
	$(SED) 's"^\"\(package/cfg/tcp_main_pem3cfg.cmd\)\"$""\"C:/Users/Risto/workspace_v5_5/debussy_rtos_tcp/.config/xconfig_tcp_main/\1\""' package/cfg/tcp_main_pem3.xdl > $@
