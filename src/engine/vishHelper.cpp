#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include "vishHelper.hpp"

namespace VishHelper {
  std::vector<char>   readBinaryFile(const std::string &filename) {
    std::ifstream	file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
      throw VishHelper::BasicError(std::string("error reading binary file ") + filename);

    size_t  fileSize = (size_t)file.tellg();
    std::vector<char>   buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    return (buffer);
  }

  VkShaderModule	createShaderModuleFromCode(VkDevice device, std::vector<char> code) {
    VkShaderModuleCreateInfo  info{};
    VkShaderModule            shaderModule;

    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<const uint32_t*>(code.data());
    if (vkCreateShaderModule(device, &info
          , nullptr, &shaderModule) != VK_SUCCESS)
      throw VishHelper::BasicError("unable to create shader module!");
    return (shaderModule);
  }
}

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
