#define CONFIG_PLATFORM_MPFS 1
#define IS_ENABLED(x) ((x) == 1)

#define HSS_HART_E51 0
#define HSS_HART_U54_1 1
#define HSS_HART_U54_2 2
#define HSS_HART_U54_3 3
#define HSS_HART_U54_4 4

#define mHSS_DEBUG_PRINTF(...)

#define CRLF "\r\n"
#define CR   "\r"
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
