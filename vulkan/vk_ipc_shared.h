#ifndef VK_IPC_SHARED_H
#define VK_IPC_SHARED_H

#include <pthread.h>
#include <stdint.h>

#define VK_IPC_QUEUE_SIZE 32

typedef enum {
  VK_IPC_NONE = 0,
  VK_IPC_vkWaitForFences,
  VK_IPC_vkAcquireNextImageKHR,
  VK_IPC_vkResetFences,
  VK_IPC_vkBeginCommandBuffer,
  VK_IPC_vkCmdBeginRenderPass,
  VK_IPC_vkCmdBindPipeline,
  VK_IPC_vkCmdSetViewport,
  VK_IPC_vkCmdSetScissor,
  VK_IPC_vkCmdDraw,
  VK_IPC_vkCmdEndRenderPass,
  VK_IPC_vkEndCommandBuffer,
  VK_IPC_vkQueueSubmit,
  VK_IPC_vkQueuePresentKHR,
} vkFunctions;

typedef struct {
  vkFunctions queue[VK_IPC_QUEUE_SIZE];
  uint32_t head;
  uint32_t tail;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  pthread_cond_t ack;
  int waiting;
  unsigned long imageIndexAddr;
  unsigned long submitInfoAddr;
  unsigned long presenInfoAddr;
} sharedMemoryQueue;

#define VK_IPC_SHM_NAME "/vk_ipc_shm"

#endif
