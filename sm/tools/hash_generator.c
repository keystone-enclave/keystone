#include <sha3/sha3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FW_MEMORY_SIZE  0x1ff000
#define HASH_SIZE       64

int main(int argc, char* argv[])
{
  if (argc != 3) {
    printf("Usage: %s <firmware> <fwsize>\n", argv[0]);
    return 0;
  }

  unsigned char sm_hash[HASH_SIZE];
  unsigned char* buf;
  FILE* fw = fopen(argv[1],"rb");
  int fwsize;

  if (!fw) {
    printf("File %s does not exist\n", argv[1]);
    return -1;
  }

  fwsize = strtol(argv[2], NULL, 16);

  // copy all file contents
  buf = (unsigned char*) malloc(FW_MEMORY_SIZE);
  memset(buf, 0, FW_MEMORY_SIZE);
  if (!buf) {
    printf("Failed to allocate buffer\n");
    return -1;
  }

  int result = fread (buf,1,fwsize,fw);
  if (result != fwsize) {
    printf("Failed to read file\n");
    return -1;
  }

  fclose(fw);

  sha3_ctx_t hash_ctx;
  sha3_init(&hash_ctx, HASH_SIZE);
  sha3_update(&hash_ctx, buf, FW_MEMORY_SIZE);
  sha3_final(sm_hash, &hash_ctx);

  printf("unsigned char sm_expected_hash[] = {");

  for (int i=0; i < HASH_SIZE; i++)
  {
    if (i % 8 == 0) {
      printf("\n");
    }
    printf("0x%.2x,", sm_hash[i]);
  }

  printf("};\n");

  printf("unsigned int sm_expected_hash_len = %d;\n", HASH_SIZE);
  return 0;
}
