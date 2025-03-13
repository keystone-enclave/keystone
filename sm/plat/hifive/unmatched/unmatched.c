#include "fu740.c"
#include "sm.h"

static int unmatched_final_init(bool cold_boot, const struct fdt_match *match) {
	sm_init(cold_boot, fdt_get_address());
	return sifive_fu740_final_init(cold_boot, match);
}

const struct platform_override hifive_unmatched = {
	.match_table = sifive_fu740_match,
	.tlbr_flush_limit = sifive_fu740_tlbr_flush_limit,
	.final_init = unmatched_final_init
};
