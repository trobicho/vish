#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>

struct  SwapChainWrapper {
  VkSwapchainKHR        chain;
  VkFormat              imageFormat;
  VkExtent2D            extent;

  std::vector<VkImage>        image;
  std::vector<VkFramebuffer>  framebuffer;
  std::vector<VkImageView>    imageView;
  uint32_t                    imageCount;
};

struct  PipelineWrapper {
  VkPipeline        pipeline;
  VkPipelineLayout  layout;
};

struct  QueueWrapper {
  VkQueue   graphics;
  uint32_t  graphicsFamilyIndex;
};

class   Vish {
    public:
      Vish(GLFWwindow *win);
      ~Vish();

      void  init();

    private:
      void  createInstance();
      void  createSurface();
      void  createSwapchain();
      void  createImageView();
      void  createLogicalDeviceAndQueue();
      void  choosePysicalDevice();
      void  createFramebuffer();

      GLFWwindow        *m_window;
      VkInstance        m_instance;
      VkDevice          m_device;
      VkPhysicalDevice  m_physicalDevice;
      VkSurfaceKHR      m_surface;
      QueueWrapper      m_queueWrap;

      SwapChainWrapper  m_swapchainWrap;

      std::vector<PipelineWrapper>   m_pipelineBuffer;

    private:
      void  createRenderPass();

      VkRenderPass    m_renderPass;

    private:
      void  createGraphicsPipeline(PipelineWrapper &pipelineWrap);
      void  destroyPipeline(PipelineWrapper &pipelineWrap);

    private:
      void  createCommandPool();
      void  allocateCommandBuffer();
      void  recordCommandBuffer();
      void  recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

      VkCommandPool                 m_commandPool;
      std::vector<VkCommandBuffer>  m_commandBuffers;
};
