#include "vish.hpp"
#include "vishParams.hpp"
#include "vishHelper.hpp"

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

  if (vkCreateInstance(&createInfo, NULL, &m_instance) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create instance");
}

void  Vish::createSurface() {
  if (glfwCreateWindowSurface(m_instance, m_win, NULL, &m_surface) != VK_SUCCESS)
    throw VishHelper::FatalVulkanInitError("Failed to create surface");
}

void  Vish::createSwapchain() {
}
