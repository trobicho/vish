#include <iostream>
#include "vish.hpp"
#include "vishParams.hpp"
#include "vishHelper.hpp"
#include "swapchainHelper.hpp"

Vish::Vish(GLFWwindow *win) : m_window(win) {
}

Vish::~Vish() {
}

void  Vish::init() {
  createInstance();
  createSurface();
  choosePysicalDevice();
  createLogicalDeviceAndQueue();
  createSwapchain();
}

void  Vish::createInstance() {
  VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "Vish",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "Vish",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_3
  };

  auto exts = VishParams::instanceExtensions();
  auto layers = VishParams::validationLayers();
  //TODO: Check valLayers exts and return VK_SUCCESS
  VkInstanceCreateInfo createInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledLayerCount = (uint32_t)layers.size(),
    .ppEnabledLayerNames = layers.data(),
    .enabledExtensionCount = (uint32_t)exts.size(),
    .ppEnabledExtensionNames = exts.data(),
  };

  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create instance");
}

void  Vish::createSurface() {
  if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create surface");
}
  
void  Vish::choosePysicalDevice() { //TODO: make the thing
  uint32_t  deviceCount;
  std::vector<VkPhysicalDevice>   device;

  vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
  device.resize(deviceCount);
  vkEnumeratePhysicalDevices(m_instance, &deviceCount, device.data());
   
  VkPhysicalDeviceProperties  props;
  vkGetPhysicalDeviceProperties(device[0], &props);
  std::cout << props.deviceName << std::endl;

  m_physicalDevice = device[0];
}

void  Vish::createLogicalDeviceAndQueue() {
  std::vector<int>                      queueIdx;
  std::vector<VkDeviceQueueCreateInfo>  queueInfo;
}

void  Vish::createSwapchain() {
  SwapchainHelper::SupportDetails details;

  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  VkExtent2D actualExtent = {
    static_cast<uint32_t>(width),
    static_cast<uint32_t>(height)
  };

  SwapchainHelper::getSupportDetails(m_physicalDevice, m_surface, details);
  auto  format = SwapchainHelper::chooseFormat(details);
  auto  presentMode = SwapchainHelper::choosePresentMode(details);
  auto  extent = SwapchainHelper::chooseExtent(details, actualExtent);

  uint32_t  imageCount = details.capability.minImageCount + 1;
  if (details.capability.maxImageCount > 0
      && imageCount > details.capability.maxImageCount)
    imageCount = details.capability.maxImageCount;

  VkSwapchainCreateInfoKHR  swapInfo = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .surface = m_surface,
    .minImageCount = imageCount,
    .imageFormat = format.format,
    .imageColorSpace = format.colorSpace,
    .imageExtent = extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = nullptr,
    .preTransform = details.capability.currentTransform,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .clipped = VK_TRUE,
    .oldSwapchain = VK_NULL_HANDLE,
  };
  if (vkCreateSwapchainKHR(m_device, &swapInfo, nullptr, &m_swapchainWrap.chain)
      != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create Swapchain!");

  if (vkGetSwapchainImagesKHR(m_device, m_swapchainWrap.chain
        , &imageCount, nullptr) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to optain imageKHR!");
  if (imageCount <= 0)
    throw VishHelper::FatalVulkanInitError("Failed to optain image count!");
  m_swapchainWrap.image.resize(imageCount);
  if (vkGetSwapchainImagesKHR(m_device, m_swapchainWrap.chain
        , &imageCount, m_swapchainWrap.image.data()) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to optain imageKHR!");
  m_swapchainWrap.imageFormat = format.format;
  m_swapchainWrap.extent = extent;
}

void  Vish::createImageView() {
  uint32_t  imageCount;

  vkGetSwapchainImagesKHR(m_device, m_swapchainWrap.chain, &imageCount, nullptr);
  m_imageView.resize(imageCount);
  for (int i = 0; i < imageCount; ++i) {
    VkImageViewCreateInfo viewInfo = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .image = m_swapchainWrap.image[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = m_swapchainWrap.imageFormat,
      .components = VkComponentMapping {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      },
      .subresourceRange = VkImageSubresourceRange {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
      },
    };
    if (vkCreateImageView(m_device, &viewInfo
        , nullptr, &m_imageView[i]) != VK_SUCCESS)
      throw VishHelper::FatalVulkanInitError("Failed to create imageViews!");
  }
}
