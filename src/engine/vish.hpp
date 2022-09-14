#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>

class Vish {
    public:
      Vish(GLFWwindow *win);
      ~Vish();

      void  init();

    private:
      void  createInstance();
      void  createSurface();
      void  createSwapchain();
      void  createImageView();

      GLFWwindow        *m_win;
      VkInstance        m_instance;
      VkDevice          m_device;
      VkPhysicalDevice  m_physicalDevice;
      VkSurfaceKHR      m_surface;

      VkSwapchainKHR              m_swapchain;
      std::vector<VkImageView>    m_imageView;
};
