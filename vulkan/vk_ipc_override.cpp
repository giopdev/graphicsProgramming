#define _GNU_SOURCE

#include "vk_ipc_shared.h"
#include <dlfcn.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <vulkan/vulkan_core.h>

static sharedMemoryQueue *shm = NULL;

static void ipcInit() {
  if (shm)
    return;
  int fd = shm_open(VK_IPC_SHM_NAME, O_RDWR, 0666);
  if (fd < 0) {
    perror("shm_open failed");
    exit(1);
  }
  shm = (sharedMemoryQueue *)mmap(NULL, sizeof(sharedMemoryQueue), PROT_READ | PROT_WRITE,
                             MAP_SHARED, fd, 0);
  if (shm == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }
  close(fd);
  fflush(stdout);
}

static void sendIpc(vkFunctions func) {
  ipcInit();

  pthread_mutex_lock(&shm->mutex);
  uint32_t next = (shm->head + 1) % VK_IPC_QUEUE_SIZE;
  while (next == shm->tail) {
    pthread_cond_wait(&shm->cond, &shm->mutex);
  }
  shm->queue[shm->head] = func;
  shm->head = next;
  shm->waiting = 1;
  pthread_cond_signal(&shm->cond);

  while (shm->waiting) {
    pthread_cond_wait(&shm->ack, &shm->mutex);
  }
  pthread_mutex_unlock(&shm->mutex);
}

/*
 * ================== Vulkan function overrides ==================
 */
extern "C" VkResult vkWaitForFences(VkDevice, uint32_t, const VkFence *,
                                    VkBool32, uint64_t) {
  sendIpc(VK_IPC_vkWaitForFences);
  return VK_SUCCESS;
}

extern "C" VkResult vkAcquireNextImageKHR(VkDevice, VkSwapchainKHR, uint64_t,
                                          VkSemaphore, VkFence,
                                          uint32_t *imageIndexPtr) {
  sendIpc(VK_IPC_vkAcquireNextImageKHR);
  if (imageIndexPtr)
    *imageIndexPtr = 0;
  return VK_SUCCESS;
}

extern "C" VkResult vkResetFences(VkDevice, uint32_t, const VkFence *) {
  sendIpc(VK_IPC_vkResetFences);
  return VK_SUCCESS;
}

extern "C" VkResult vkBeginCommandBuffer(VkCommandBuffer,
                                         const VkCommandBufferBeginInfo *) {
  sendIpc(VK_IPC_vkBeginCommandBuffer);
  return VK_SUCCESS;
}

extern "C" void vkCmdBeginRenderPass(VkCommandBuffer,
                                     const VkRenderPassBeginInfo *,
                                     VkSubpassContents) {
  sendIpc(VK_IPC_vkCmdBeginRenderPass);
}

extern "C" void vkCmdBindPipeline(VkCommandBuffer, VkPipelineBindPoint,
                                  VkPipeline) {
  sendIpc(VK_IPC_vkCmdBindPipeline);
}

extern "C" void vkCmdSetViewport(VkCommandBuffer, uint32_t, uint32_t,
                                 const VkViewport *) {
  sendIpc(VK_IPC_vkCmdSetViewport);
}

extern "C" void vkCmdSetScissor(VkCommandBuffer, uint32_t, uint32_t,
                                const VkRect2D *) {
  sendIpc(VK_IPC_vkCmdSetScissor);
}

extern "C" void vkCmdDraw(VkCommandBuffer, uint32_t, uint32_t, uint32_t,
                          uint32_t) {
  sendIpc(VK_IPC_vkCmdDraw);
}

extern "C" void vkCmdEndRenderPass(VkCommandBuffer) {
  sendIpc(VK_IPC_vkCmdEndRenderPass);
}

extern "C" VkResult vkEndCommandBuffer(VkCommandBuffer) {
  sendIpc(VK_IPC_vkEndCommandBuffer);
  return VK_SUCCESS;
}

extern "C" VkResult vkQueueSubmit(VkQueue, uint32_t,
                                  const VkSubmitInfo *submitInfoPtr, VkFence) {
  // std::cout << "command buffer count --- [" << submitInfo->commandBufferCount
  //           << "]\n";
  // fflush(stdout);
  shm->submitInfoAddr = (unsigned long)submitInfoPtr;
  sendIpc(VK_IPC_vkQueueSubmit);
  return VK_SUCCESS;
}

extern "C" VkResult vkQueuePresentKHR(VkQueue, const VkPresentInfoKHR *) {
  sendIpc(VK_IPC_vkQueuePresentKHR);
  return VK_SUCCESS;
}
