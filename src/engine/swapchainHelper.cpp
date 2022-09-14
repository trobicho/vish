#include "swapchainHelper.hpp"
#include "vishHelper.hpp"
#include <algorithm>
#include <limits>

namespace SwapchainHelper {
  void  getSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface, SupportDetails &details) {
    uint32_t  count;

    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface
        , &details.capability) != VK_SUCCESS)
      throw VishHelper::BasicError("Failed to get device surface capability");
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
    if (count != 0) {
      details.formats.resize(count);
      if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count
            , details.formats.data()) != VK_SUCCESS)
        throw VishHelper::BasicError("Failed to get device surface formats");
    }
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
    if (count != 0) {
      details.presentModes.resize(count);
      if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count
            , details.presentModes.data()) != VK_SUCCESS)
        throw VishHelper::BasicError("Failed to get device surface present modes");
    }
  }

  VkSurfaceFormatKHR  chooseFormat(SupportDetails &details) {
    if (details.formats.size() == 1
        && details.formats[0].format == VK_FORMAT_UNDEFINED) {
      return ((VkSurfaceFormatKHR){
        VK_FORMAT_B8G8R8_UNORM,
        VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
      });
    }
    for (auto format : details.formats) {
      if (format.format == VK_FORMAT_B8G8R8A8_UNORM
          && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        return (format);
    }
    return (details.formats[0]);
  }

  VkExtent2D  chooseExtent(SupportDetails &details, VkExtent2D actualExtent) {
    if (details.capability.currentExtent.width
        != std::numeric_limits<uint32_t>::max())
      return (details.capability.currentExtent);

    actualExtent = (VkExtent2D){
      .width = std::max(
        std::min(actualExtent.width, details.capability.maxImageExtent.width)
        , details.capability.minImageExtent.width),
      .height = std::max(
        std::min(actualExtent.height, details.capability.maxImageExtent.height)
        , details.capability.minImageExtent.height),
    };
    return (actualExtent);
  }
}
