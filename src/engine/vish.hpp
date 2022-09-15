#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>

struct  SwapChainWrapper {
  VkSwapchainKHR        chain;
  std::vector<VkImage>  image;
  VkFormat              imageFormat;
  VkExtent2D            extent;
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

      GLFWwindow        *m_window;
      VkInstance        m_instance;
      VkDevice          m_device;
      VkPhysicalDevice  m_physicalDevice;
      VkSurfaceKHR      m_surface;
      QueueWrapper      m_queueWrap;

      SwapChainWrapper            m_swapchainWrap;
      std::vector<VkImageView>    m_imageView;
};
