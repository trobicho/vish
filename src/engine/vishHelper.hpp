#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <string>

namespace VishHelper {
  void  infoInstance();

  class	FatalError: public std::exception
  {
    public:
      //FatalError(const string& msg);
      FatalError(std::string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      std::string message;
  };

  class	FatalVulkanInitError: public std::exception
  {
    public:
      FatalVulkanInitError(std::string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      std::string message;
  };

  class	BasicError: public std::exception
  {
    public:
      BasicError(std::string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      std::string message;
  };
}