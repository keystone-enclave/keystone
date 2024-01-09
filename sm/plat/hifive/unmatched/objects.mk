
carray-platform_override_modules-y += hifive_unmatched
platform-objs-y += ../hifive/unmatched/unmatched.o
platform-cflags-y += -I$(src_dir)/platform/generic/sifive
