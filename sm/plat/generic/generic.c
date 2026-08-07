
#include <platform_override.h>
#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/fdt/fdt_fixup.h>

#include "sm.h"

static int generic_final_init(bool cold_boot, const struct fdt_match *match) {
        sm_init(cold_boot, sbi_scratch_thishart_arg1_ptr());
        return 0;
}

static const struct fdt_match generic_match[] = {
	{ .compatible = "riscv-virtio" },
	{ .compatible = "riscv-virtio,qemu" },
	{ },
};

const struct platform_override generic = {
	.match_table = generic_match,
        .final_init = generic_final_init
};
