#include "./common.h"
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  int fd = open("/dev/sharedgl_mapproc", O_RDWR);
  if (fd < 0) {
    perror("Failed to open device");
    return -1;
  }

#ifdef TWO_WAY
  int fd_rev = open("/dev/sharedgl_mapproc_rev", O_RDWR);
  if (fd_rev < 0) {
    perror("Failed to open device");
    return -1;
  }

  if (ioctl(fd_rev, IOCTL_REG_GUEST) == -1) {
    perror("ioctl failed");
    close(fd_rev);
    return -1;
  }
  printf("[*] Registered the Game process as the guest successfully\n");

#endif

  /* Register as GUEST */
  if (ioctl(fd, IOCTL_REG_HOST) == -1) {
    perror("ioctl failed to register Game");
    close(fd);
    return -1;
  }
  printf("[*] Registered the Game process as the host successfully\n");

  printf("Press <enter> to load tri...\n");
  char dummy_input;
  scanf("%c", &dummy_input);

  syscall(sys_exec_sg, argv[1], argv, envp);
  return 0;
}
