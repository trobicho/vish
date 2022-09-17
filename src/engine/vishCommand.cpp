#include "vish.hpp"
#include "vishHelper.hpp"

void  Vish::createCommandPool() {
  VkCommandPoolCreateInfo poolInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .flags = 0,
    .queueFamilyIndex = m_queueWrap.graphicsFamilyIndex,
  };
  if (vkCreateCommandPool(m_device, &poolInfo
        , nullptr, &m_commandPool) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create Command Pool !");
}

void  Vish::allocateCommandBuffer() {
  m_commandBuffers.resize(m_swapchainWrap.imageCount);
  VkCommandBufferAllocateInfo allocateInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .commandPool = m_commandPool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = m_swapchainWrap.imageCount,
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
  vkCmdDraw(commandBuffer, 3, 1, 0, 0);
  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to record Command Buffer !");
}
