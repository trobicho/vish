#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

class Vish {
    public:
      Vish(GLFWwindow *win);
      ~Vish();

      void  init();

    private:
      void  createInstance();
      void  createSurface();
      void  createSwapchain();

      GLFWwindow      *m_win;
      VkInstance      m_instance;
      VkSurfaceKHR    m_surface;
};
