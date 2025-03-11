#include "jh7110.c"
#include "sm.h"

static int visionfive2_final_init(bool cold_boot, const struct fdt_match *match) {
	sm_init(cold_boot);
	return starfive_jh7110_final_init(cold_boot, match);
}

const struct platform_override visionfive2 = {
	.match_table = starfive_jh7110_match,
	.cold_boot_allowed = starfive_jh7110_cold_boot_allowed,
	.fw_init = starfive_jh7110_fw_init,
	.final_init = visionfive2_final_init
};
