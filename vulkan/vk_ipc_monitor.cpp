#include "common.h"
#include "vk_ipc_shared.h"
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <vulkan/vulkan_core.h>

static const int TIME_TO_LOOP = 120;

const char *vkFunctionToString(vkFunctions func) {
  switch (func) {
  case VK_IPC_vkWaitForFences:
    return "vkWaitForFences";
  case VK_IPC_vkAcquireNextImageKHR:
    return "vkAcquireNextImageKHR";
  case VK_IPC_vkResetFences:
    return "vkResetFences";
  case VK_IPC_vkBeginCommandBuffer:
    return "vkBeginCommandBuffer";
  case VK_IPC_vkCmdBeginRenderPass:
    return "vkCmdBeginRenderPass";
  case VK_IPC_vkCmdBindPipeline:
    return "vkCmdBindPipeline";
  case VK_IPC_vkCmdSetViewport:
    return "vkCmdSetViewport";
  case VK_IPC_vkCmdSetScissor:
    return "vkCmdSetScissor";
  case VK_IPC_vkCmdDraw:
    return "vkCmdDraw";
  case VK_IPC_vkCmdEndRenderPass:
    return "vkCmdEndRenderPass";
  case VK_IPC_vkEndCommandBuffer:
    return "vkEndCommandBuffer";
  case VK_IPC_vkQueueSubmit:
    return "vkQueueSubmit";
  case VK_IPC_vkQueuePresentKHR:
    return "vkQueuePresentKHR";
  default:
    return "Unknown";
  }
}

int main() {
  char c;

  // Reset shmem
  shm_unlink(VK_IPC_SHM_NAME);

  int fd = shm_open(VK_IPC_SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (fd < 0) {
    perror("shm_open failed");
    return 1;
  }
  ftruncate(fd, sizeof(sharedMemoryQueue));
  sharedMemoryQueue *shm = (sharedMemoryQueue *)mmap(
      NULL, sizeof(sharedMemoryQueue), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shm == MAP_FAILED) {
    perror("mmap failed");
    return 1;
  }
  close(fd);

  int mapproc_fd_rev, mapproc_fd;
  mapproc_fd_rev = open("/dev/sharedgl_mapproc_rev", O_RDWR);
  if (mapproc_fd_rev < 0) {
    perror("Failed to open device");
    return -1;
  }

  mapproc_fd = open("/dev/sharedgl_mapproc", O_RDWR);
  if (mapproc_fd < 0) {
    perror("Failed to open device");
    return -1;
  }
  if (ioctl(mapproc_fd_rev, IOCTL_REPLAY_MAPPINGS) == WAIT) {
    perror("ioctl failed");
    close(mapproc_fd_rev);
    return -1;
  }
  printf("[*] Replayed all mappings.\n");

  if (ioctl(mapproc_fd_rev, IOCTL_MAP_DATA_STACK) == WAIT) {
    perror("ioctl failed");
    close(mapproc_fd_rev);
    return -1;
  }

  if (ioctl(mapproc_fd, IOCTL_MAP_DATA_STACK) == WAIT) {
    perror("ioctl failed");
    close(mapproc_fd);
    return -1;
  }

  if (shm->head == 0 && shm->tail == 0) {
    pthread_mutexattr_t mattr;
    pthread_condattr_t cattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm->mutex, &mattr);
    pthread_cond_init(&shm->cond, &cattr);
    pthread_cond_init(&shm->ack, &cattr);

    pthread_mutexattr_destroy(&mattr);
    pthread_condattr_destroy(&cattr);

    printf("Shared memory queue initialized\n");
    fflush(stdout);
  }

  printf("Waiting for Vulkan function calls...\n");
  fflush(stdout);

  std::cout << "Press <enter> to loop..." << std::endl;
  scanf("%c", &c);

  using clock = std::chrono::steady_clock;
  auto start = clock::now();

  while (1) {
    auto now = clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
    if (elapsed >= TIME_TO_LOOP) {
      printf("%d seconds elapsed, exiting loop.\n", TIME_TO_LOOP);
      goto cleanup;
    }

    pthread_mutex_lock(&shm->mutex);
    while (shm->head == shm->tail) {
      pthread_cond_wait(&shm->cond, &shm->mutex);
    }
    vkFunctions func = shm->queue[shm->tail];
    shm->tail = (shm->tail + 1) % VK_IPC_QUEUE_SIZE;
    pthread_cond_signal(&shm->cond);

    VkSubmitInfo *submitInfo = nullptr;

    switch (func) {
    case VK_IPC_vkAcquireNextImageKHR:
      break;
    case VK_IPC_vkQueueSubmit:
      submitInfo = (VkSubmitInfo *)shm->submitInfoAddr;
      if (submitInfo != nullptr) {
        printf("submitInfo good: %p\n", submitInfo);
        fflush(stdout);
        std::cout << "Command buffer count: [" << submitInfo->commandBufferCount
                  << "]" << std::endl;
        fflush(stdout);
      } else {
        std::cout << "submitInfoptr == nullptr\n";
        fflush(stdout);
      }
      break;
    case VK_IPC_vkQueuePresentKHR:
      break;
    default:
      break;
      // std::cout << "Unknown function [" << func << "]!\n";
      // goto cleanup;
    }

    printf("Intercepted: %s\n", vkFunctionToString(func));
    fflush(stdout);

    shm->waiting = 0;
    pthread_cond_signal(&shm->ack);
    pthread_mutex_unlock(&shm->mutex);
  }

cleanup:
#ifdef TWO_WAY
  int fd_rev = open("/dev/sharedgl_mapproc_rev", O_RDWR);
  if (fd_rev < 0) {
    perror("Failed to open device");
    return -1;
  }
#endif

  if (ioctl(fd, IOCTL_UNREG) == -1) {
    perror("Monitor tried to Unregister, but failed!\n");
  } else
    printf("Unreg on regular module\n");

#ifdef TWO_WAY
  if (ioctl(fd_rev, IOCTL_UNREG) == -1) {
    perror("[REV] Monitor tried to Unregister, but failed!\n");
  } else
    printf("Unreg on reverse module\n");
#endif

  return 0;
}
