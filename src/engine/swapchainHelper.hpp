#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace SwapchainHelper {
  struct  SupportDetails {
    VkSurfaceCapabilitiesKHR          capability;
    std::vector<VkSurfaceFormatKHR>   formats;
    std::vector<VkPresentModeKHR>     presentModes;
  };

  void  getSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface, SupportDetails &details);

  VkSurfaceFormatKHR  chooseFormat(SupportDetails &details);
  VkPresentModeKHR    choosePresentMode(SupportDetails &details) {
    return (VK_PRESENT_MODE_FIFO_KHR);
  }
  VkExtent2D          chooseExtent(SupportDetails &details, VkExtent2D actualExtent);
}
