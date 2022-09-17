#include "vish.hpp"
#include "vishHelper.hpp"

void  Vish::createCommandPool() {
  VkCommandPoolCreateInfo poolInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = m_queueWrap.graphicsFamilyIndex,
  };
  if (vkCreateCommandPool(m_device, &poolInfo
        , nullptr, &m_commandPool) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create Command Pool !");
}

void  Vish::allocateCommandBuffer() {
  m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  VkCommandBufferAllocateInfo allocateInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .commandPool = m_commandPool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = (uint32_t) m_commandBuffers.size(),
  };
  if (vkAllocateCommandBuffers(m_device, &allocateInfo
        , m_commandBuffers.data()) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to allocate Command Buffers !");
}

void  Vish::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
  VkCommandBufferBeginInfo  beginInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = 0,
    .pInheritanceInfo = nullptr,
  };

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to begin recording Command Buffer !");
  VkClearValue clearValue = (VkClearValue){0.0f, 0.0f, 0.0f, 1.0f};
  VkRenderPassBeginInfo     renderPassInfo = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .renderPass = m_renderPass,
    .framebuffer = m_swapchainWrap.framebuffer[imageIndex],
    .renderArea = (VkRect2D) {
      .offset = (VkOffset2D){0, 0},
      .extent = m_swapchainWrap.extent,
    },
    .clearValueCount = 1,
    .pClearValues = &clearValue,
  };

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS
      , m_pipelineBuffer[0].pipeline);

  VkViewport viewport{
    .x = 0.0f,
    .y = 0.0f,
    .width = static_cast<float>(m_swapchainWrap.extent.width),
    .height= static_cast<float>(m_swapchainWrap.extent.height),
    .minDepth = 0.0f,
    .maxDepth = 1.0f,
  };
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{
    .offset = {0, 0},
    .extent = m_swapchainWrap.extent,
  };
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdDraw(commandBuffer, 4, 1, 0, 0);
  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to end recording Command Buffer !");
}

void  Vish::createRenderSync() {
  m_renderSync.currentFrame = 0;
  m_renderSync.semaphoreAvailable.resize(MAX_FRAMES_IN_FLIGHT);
  m_renderSync.semaphoreFinish.resize(MAX_FRAMES_IN_FLIGHT);
  m_renderSync.fence.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
  };
  VkFenceCreateInfo     fenceInfo = {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr
          , &m_renderSync.semaphoreAvailable[i]) != VK_SUCCESS
        || vkCreateSemaphore(m_device, &semaphoreInfo, nullptr
          , &m_renderSync.semaphoreFinish[i]) != VK_SUCCESS
        || vkCreateFence(m_device, &fenceInfo, nullptr
          , &m_renderSync.fence[i]) != VK_SUCCESS)
      throw VishHelper::FatalVulkanInitError("Failed to create RenderSync !");
  }
}
