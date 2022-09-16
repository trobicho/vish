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
};

struct  QueueWrapper {
  VkQueue   graphics;
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

      SwapChainWrapper            m_swapchainWrap;

    private:
      void  createRenderPass();

      VkRenderPass    m_renderPass;
};
