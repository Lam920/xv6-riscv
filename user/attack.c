#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define SECRET_LENGTH 8

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)

  char secret[SECRET_LENGTH];
  char *end = sbrk(PGSIZE*32);
  memset(secret, 0, SECRET_LENGTH);
  end = end + 16 * PGSIZE;
  // Do not know why first 8 bytes is written is clear ??? ~ PTE ???
  memmove(secret, end + 32, SECRET_LENGTH);
  write(2, secret, 8);
  exit(1);
}
