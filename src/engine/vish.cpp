#include "vish.hpp"
#include "vishParams.hpp"
#include "vishHelper.hpp"
#include "swapchainHelper.hpp"

Vish::Vish(GLFWwindow *win) : m_win(win) {
}

Vish::~Vish() {
}

void  Vish::init() {
  createInstance();
  createSurface();
  //physicalDev
  //logicalDev_queue
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
  if (glfwCreateWindowSurface(m_instance, m_win, nullptr, &m_surface) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create surface");
}

void  Vish::createSwapchain() {
}

void  Vish::createImageView() {
  uint32_t  imageCount;

  vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr);
  m_imageView.resize(imageCount);
  for (int i = 0; i < imageCount; ++i) {
    VkImageViewCreateInfo viewInfo = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO.
      .image = m_swapchainImage[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = m_swapchainImageFormat,
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
