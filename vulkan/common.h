#ifndef COMMON_H
#define COMMON_H

#define MAX_ARGS 20

#define TWO_WAY

#define sys_exec_sg 548

#define IOCTL_REG_HOST _IO('m', 5)
#define IOCTL_REG_GUEST _IO('m', 6)
#define IOCTL_UNREG _IO('m', 7)
#define IOCTL_STOP _IO('m', 8)
#define REMAP_RW_REGION _IO('m', 15)

#define IOCTL_TEST_START _IO('m', 9)
#define IOCTL_TEST_COMPLETE _IO('m', 10)

#define IOCTL_TEST_STATUS _IO('m', 11)
#define IOCTL_CLR_MAPPING _IO('m', 12)
#define IOCTL_MAP_DATA_STACK _IO('m', 13)
#define IOCTL_DUMP_PTE _IOW('m', 14, unsigned long)
#define IOCTL_REPLAY_MAPPINGS _IO('m', 16)

#define WAIT -1

#endif
