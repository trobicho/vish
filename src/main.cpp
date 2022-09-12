#include "engine/vish.hpp"
#include "engine/vishHelper.hpp"

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow *win = glfwCreateWindow(800, 800, "Vish", NULL, NULL);

  Vish  vish(win);

  VishHelper::infoInstance();
  vish.init();

  while(!glfwWindowShouldClose(win)) {
    glfwPollEvents();
  }
  return (0);
}
