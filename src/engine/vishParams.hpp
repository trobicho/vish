#pragma once
#include <vector>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace VishParams {
  inline std::vector<const char*>  validationLayers() {
    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};
    return (layers);
  }

  inline std::vector<const char*>  instanceExtensions() {
    std::vector<const char*> exts = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

    uint32_t glfwExtCount; 
    const char **glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    for (int i = 0; i < glfwExtCount; ++i) {
      exts.push_back(glfwExt[i]);
    }
    return (exts);
  }

  inline std::vector<const char*>  deviceExtensions() {
    std::vector<const char*> exts = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    return (exts);
  }
}
