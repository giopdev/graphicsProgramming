# Vulkan vs OpenGL

## Initial info

### GLFW

GLFW is a thin, multi-platform abstraction layer for creating and managing
windows, handling input, and managing OpenGL and Vulkan contexts.

### tl;dr

GLFW lets us focus on OpenGL or Vulkan by providing platform-agnostic windows
to render in.

<div style="page-break-after: always;"></div>

# OpenGL vs Vulkan : High Level

## Vulkan is far less abstracted.
---

### libraryTrace : `ltrace ./main.bin`

_**ltrace only tracks dynamic links, slightly misleading.. since OpenGL entry
points are statically linked here**_

_Although, there wouldn't be much more to show for OpenGL_

### OpenGL

```cpp
glfwWindowShouldClose(0x56d8443a65e0, 0x70602ac3b8e8, 1, 0) /* loop Condition */
glfwPollEvents(0x70600d535010, 0, 530, 0)

glfwSwapBuffers(0x56d8443a65e0, 0, 1, 0)
```

### Vulkan

```cpp
glfwWindowShouldClose(0x5f2b32cf0300, 0x5f2b32c33010, 1, 0) /* loop Condition */
glfwPollEvents(0x5f2b32cf0300, 0x5f2b32c33010, 1, 0)

vkWaitForFences(0x5f2b32f1cd50, 1, 0x5f2b32fdaa08, 1)
vkAcquireNextImageKHR(0x5f2b32f1cd50, 0x1c000000001c, -1, 0x190000000019)
vkResetFences(0x5f2b32f1cd50, 1, 0x5f2b32fdaa08, 0)
vkBeginCommandBuffer(0x5f2b32f5bf50, 0x7ffd6263a570, 0x7ffd6263a570, 0x5f2b32f5bf50)
vkCmdBeginRenderPass(0x5f2b32f5bf50, 0x7ffd6263a590, 0, 0x7ffd6263a590)
vkCmdBindPipeline(0x5f2b32f5bf50, 0, 0x100000000010, 0x5f2b32f61c18)
vkCmdSetViewport(0x5f2b32f5bf50, 0, 1, 0x7ffd6263a550)
vkCmdSetScissor(0x5f2b32f5bf50, 0, 1, 0x7ffd6263a540)
vkCmdDraw(0x5f2b32f5bf50, 3, 1, 0)
vkCmdEndRenderPass(0x5f2b32f5bf50, 0, 1, 0)
vkEndCommandBuffer(0x5f2b32f5bf50, 0xffffff80, 1, 0)

vkQueueSubmit(0x5f2b32c5d4e0, 1, 0x7ffd6263a670, 0x1b000000001b)
vkQueuePresentKHR(0x5f2b32c5d4e0, 0x7ffd6263a630, 0x7ffd6263a630, 0x7ffd6263a698)
```
<div style="page-break-after: always;"></div>

## Vulkan is far less abstracted.
---

## Strace Comparison

### Vulkan `strace ./main.bin`

```c
/*
 * RENDER LOOP
 */
ppoll([{fd=3, events=POLLIN}], 1, {tv_sec=0, tv_nsec=0}, NULL, 8) = 1 ([{fd=3, revents=POLLIN}], left {tv_sec=0, tv_nsec=0})
recvmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="e\0\0\0\0\0\10\0", iov_len=2736}, {iov_base="", iov_len=1360}], msg_iovlen=2, msg_controllen=0, msg_flags=MSG_CMSG_CLOEXEC}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = 8
recvmsg(3, {msg_namelen=0}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = -1 EAGAIN (Resource temporarily unavailable)
ppoll([{fd=3, events=POLLIN}], 1, {tv_sec=0, tv_nsec=0}, NULL, 8) = 0 (Timeout)
sendmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="I\0\0\0\1\0\24\0n\0\0\0\0\0\0\0\311\3\0\0I\0\0\0\2\0\24\0o\0\0\0"..., iov_len=92}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, MSG_DONTWAIT|MSG_NOSIGNAL) = 92
ppoll([{fd=3, events=POLLIN}, {fd=14, events=POLLIN}, {fd=19, events=POLLIN}, {fd=3, events=POLLIN}], 4, {tv_sec=0, tv_nsec=0}, NULL, 8) = 0 (Timeout)
ioctl(18, DRM_IOCTL_SYNCOBJ_WAIT, 0x7ffe2fb57190) = 0
futex(0x5ecf24520d60, FUTEX_WAKE_PRIVATE, 1) = 1
futex(0x5ecf24520d18, FUTEX_WAKE_PRIVATE, 1) = 1
futex(0x5ecf243a5a64, FUTEX_WAKE_PRIVATE, 1) = 1
ioctl(18, DRM_IOCTL_SYNCOBJ_QUERY, 0x7ffe2fb571f0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_TIMELINE_WAIT, 0x7ffe2fb571b0) = -1 ETIME (Timer expired)
ioctl(18, DRM_IOCTL_SYNCOBJ_CREATE, 0x7ffe2fb573c0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_TRANSFER, 0x7ffe2fb573b0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_HANDLE_TO_FD, 0x7ffe2fb573b0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_CREATE, 0x7ffe2fb573c0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_TRANSFER, 0x7ffe2fb573b0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_HANDLE_TO_FD, 0x7ffe2fb573b0) = 0
ioctl(23, SYNC_IOC_MERGE, 0x7ffe2fb57430) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_CREATE, 0x7ffe2fb57330) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_FD_TO_HANDLE, 0x7ffe2fb573a0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_DESTROY, 0x7ffe2fb573d0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_DESTROY, 0x7ffe2fb573d0) = 0
close(23)                               = 0
close(24)                               = 0
close(25)                               = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_RESET, 0x7ffe2fb573a0) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_TIMELINE_WAIT, 0x7ffe2fb56160) = 0
ioctl(18, DRM_IOCTL_AMDGPU_CS, 0x7ffe2fb55d00) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_DESTROY, 0x7ffe2fb56250) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_WAIT, 0x7ffe2fb57050) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_RESET, 0x7ffe2fb57240) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_TIMELINE_WAIT, 0x7ffe2fb56880) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_HANDLE_TO_FD, 0x7ffe2fb56430) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_HANDLE_TO_FD, 0x7ffe2fb56430) = 0
ioctl(23, SYNC_IOC_MERGE, 0x7ffe2fb566d0) = 0
close(23)                               = 0
close(24)                               = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_FD_TO_HANDLE, 0x7ffe2fb56440) = 0
close(25)                               = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_TRANSFER, 0x7ffe2fb56430) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_TRANSFER, 0x7ffe2fb56430) = 0
ioctl(18, DRM_IOCTL_SYNCOBJ_RESET, 0x7ffe2fb569a0) = 0
```

### OpenGL `strace ./main.bin`

```c
/*
 * RENDER LOOP
 */
getpid()                                = 69982
ioctl(20, DRM_IOCTL_SYNCOBJ_CREATE, 0x7ffd8871bcf0) = 0
futex(0x62379a4629f4, FUTEX_WAKE_PRIVATE, 1) = 1
futex(0x62379a4629a0, FUTEX_WAKE_PRIVATE, 1) = 1
futex(0x62379a69590c, FUTEX_WAIT_BITSET, 2, NULL, FUTEX_BITSET_MATCH_ANY) = -1 EAGAIN (Resource temporarily unavailable)
poll([{fd=3, events=POLLIN}], 1, -1)    = 1 ([{fd=3, revents=POLLIN}])
recvmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="6\0\0\0\0\0\10\0", iov_len=2532}, {iov_base="", iov_len=1564}], msg_iovlen=2, msg_controllen=0, msg_flags=MSG_CMSG_CLOEXEC}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = 8
recvmsg(3, {msg_namelen=0}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = -1 EAGAIN (Resource temporarily unavailable)
poll([{fd=3, events=POLLIN}], 1, -1)    = 1 ([{fd=3, revents=POLLIN}])
recvmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="7\0\0\0\0\0\f\0%\316v\3\1\0\0\0\1\0\f\0007\0\0\0", iov_len=2524}, {iov_base="", iov_len=1572}], msg_iovlen=2, msg_controllen=0, msg_flags=MSG_CMSG_CLOEXEC}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = 24
recvmsg(3, {msg_namelen=0}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = -1 EAGAIN (Resource temporarily unavailable)
sendmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="$\0\0\0\3\0\f\0007\0\0\0$\0\0\0\1\0\24\0-\0\0\0\0\0\0\0\0\0\0\0"..., iov_len=64}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, MSG_DONTWAIT|MSG_NOSIGNAL) = 64
ppoll([{fd=3, events=POLLIN}, {fd=14, events=POLLIN}, {fd=19, events=POLLIN}, {fd=3, events=POLLIN}], 4, {tv_sec=0, tv_nsec=0}, NULL, 8) = 0 (Timeout)
```
<div style="page-break-after: always;"></div>

### OpenGL `strace -f ./main.bin`
```c
/*
 * RENDER LOOP
 */
77954 getpid()                          = 77954
77954 ioctl(20, DRM_IOCTL_SYNCOBJ_CREATE, 0x7ffe603ff1d0) = 0
77954 futex(0x58dd0e96d4e0, FUTEX_WAKE_PRIVATE, 1) = 1
77960 <... futex resumed>)              = 0
77954 futex(0x58dd0ee7fe3c, FUTEX_WAIT_BITSET, 2, NULL, FUTEX_BITSET_MATCH_ANY <unfinished ...>
77960 futex(0x58dd0e96d490, FUTEX_WAKE_PRIVATE, 1) = 0
77960 ioctl(20, DRM_IOCTL_SYNCOBJ_DESTROY, 0x7f8aa8dfec30) = 0
77960 ioctl(20, DRM_IOCTL_AMDGPU_CS, 0x7f8aa8dfeab0) = 0
77960 futex(0x58dd0ee7fe3c, FUTEX_WAKE, 2147483647 <unfinished ...>
77954 <... futex resumed>)              = 0
77960 <... futex resumed>)              = 1
77954 poll([{fd=3, events=POLLIN}], 1, -1 <unfinished ...>
77960 futex(0x58dd0e96d4e4, FUTEX_WAIT_BITSET_PRIVATE|FUTEX_CLOCK_REALTIME, 697, NULL, FUTEX_BITSET_MATCH_ANY <unfinished ...>
77954 <... poll resumed>)               = 1 ([{fd=3, revents=POLLIN}])
77954 recvmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="6\0\0\0\0\0\10\0", iov_len=2508}, {iov_base="", iov_len=1588}], msg_iovlen=2, msg_controllen=0, msg_flags=MSG_CMSG_CLOEXEC}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = 8
77954 recvmsg(3, {msg_namelen=0}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = -1 EAGAIN (Resource temporarily unavailable)
77954 poll([{fd=3, events=POLLIN}], 1, -1) = 1 ([{fd=3, revents=POLLIN}])
77954 recvmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="7\0\0\0\0\0\f\0001\25\214\3\1\0\0\0\1\0\f\0007\0\0\0", iov_len=2500}, {iov_base="", iov_len=1596}], msg_iovlen=2, msg_controllen=0, msg_flags=MSG_CMSG_CLOEXEC}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = 24
77954 recvmsg(3, {msg_namelen=0}, MSG_DONTWAIT|MSG_CMSG_CLOEXEC) = -1 EAGAIN (Resource temporarily unavailable)
77954 sendmsg(3, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="$\0\0\0\3\0\f\0007\0\0\0$\0\0\0\1\0\24\0-\0\0\0\0\0\0\0\0\0\0\0"..., iov_len=64}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, MSG_DONTWAIT|MSG_NOSIGNAL) = 64
77954 ppoll([{fd=3, events=POLLIN}, {fd=14, events=POLLIN}, {fd=19, events=POLLIN}, {fd=3, events=POLLIN}], 4, {tv_sec=0, tv_nsec=0}, NULL, 8) = 0 (Timeout)
```
<div style="page-break-after: always;"></div>

# Work Submission to the GPU (Vulkan)
```c
// Your Program Calls ->>
vkQueueSubmit(0x5f2b32c5d4e0, 1, 0x7ffd6263a670, 0x1b000000001b);
// calls ->> (Mesa)
    radv_queue_submit_normal(struct radv_queue *queue, struct vk_queue_submit *submission);
// calls ->> (libdrm)
        drm_public int amdgpu_cs_submit(amdgpu_context_handle context,
                                        uint64_t flags,
                                        struct amdgpu_cs_request *ibs_request,
                                        uint32_t number_of_requests);
// calls ->> (libdrm)
            static int amdgpu_cs_submit_one(amdgpu_context_handle context,
                                            struct amdgpu_cs_request *ibs_request);
// calls ->> (libdrm)
                drm_public int amdgpu_cs_submit_raw2(amdgpu_device_handle dev,
                                                     amdgpu_context_handle context,
                                                     uint32_t bo_list_handle,
                                                     int num_chunks,
                                                     struct drm_amdgpu_cs_chunk *chunks,
                                                     uint64_t *seq_no);
// calls ->> (libdrm)
                    drm_public int drmCommandWriteRead(int fd, unsigned long drmCommandIndex,
                                                       void *data, unsigned long size);
                        /* Performs ioctl */
```

<div style="page-break-after: always;"></div>

# Tangential Stuff That I learned

### Vulkan has a headless mode

### Vulkan pipelines are nearly immutable
