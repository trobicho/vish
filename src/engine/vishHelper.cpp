#include <iostream>
#include <cstdint>
#include <string>
#include "vishHelper.hpp"

namespace VishHelper {
  void  infoInstance() {
    uint32_t apiVersion;

    vkEnumerateInstanceVersion(&apiVersion);
    std::cout << "API version = " << VK_API_VERSION_VARIANT(apiVersion) << " "
        << VK_API_VERSION_MAJOR(apiVersion) << "."
        << VK_API_VERSION_MINOR(apiVersion) << "."
        << VK_API_VERSION_PATCH(apiVersion) << std::endl;
  }
}
