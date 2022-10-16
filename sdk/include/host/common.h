#ifndef __COMMON_H__
#define __COMMON_H__

#define RT_NOEXEC 0
#define USER_NOEXEC 1
#define RT_FULL 2
#define USER_FULL 3
#define UTM_FULL 4

#define PAGE_BITS 12
#define PAGE_SIZE (1UL << PAGE_BITS)
#define ROUND_UP(n, b) (((((n)-1ul) >> (b)) + 1ul) << (b))
#define ROUND_DOWN(n, b) (n & ~((2 << (b - 1)) - 1))
#define PAGE_DOWN(n) ROUND_DOWN(n, PAGE_BITS)
#define PAGE_UP(n) ROUND_UP(n, PAGE_BITS)

#define BOOST_STRINGIZE(X) BOOST_DO_STRINGIZE(X)
#define BOOST_DO_STRINGIZE(X) #X

#define KEYSTONE_DEV_PATH "/dev/keystone_enclave"

#define MSG(str) \
  "[Keystone SDK] " __FILE__ ":" BOOST_STRINGIZE(__LINE__) " : " str
#define ERROR(str, ...) fprintf(stderr, MSG(str) "\n", ##__VA_ARGS__)
#define PERROR(str) perror(MSG(str))
#define IS_ALIGNED(x, align) (!((x) & (align - 1)))

/* Currently we have only one command avaiable from the enclave.
 * We need to move it out to other header file (e.g., keystone-sm.h) */
#define KEYSTONE_ENCLAVE_DONE 0
#define KEYSTONE_ENCLAVE_INTERRUPTED 100002
#define KEYSTONE_ENCLAVE_EDGE_CALL_HOST 100011

#endif
