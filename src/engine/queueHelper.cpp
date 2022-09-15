#include "queueHelper.hpp"
#include <vector>

namespace QueueHelper {
  void  familyIndex(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t *queueFamily) {
    uint32_t queueCount;
    VkBool32  presentSupport;

    std::vector<VkQueueFamilyProperties> queueProps;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);
    if (queueCount == 0)
      throw VishHelper::FatalVulkanInitError("No Queue found !");
    queueProps.resize(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueProps.data());
    for (int i = 0; i < queueCount; ++i) {
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
      if (queueProps[i].queueCount > 0
          && queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT
          && presentSupport) {
        *queueFamily = i;
        return ;
      }
    }
    throw VishHelper::FatalVulkanInitError("No Queue found !");
  }
}
