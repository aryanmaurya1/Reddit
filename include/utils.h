#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

GLFWwindow *GetWindow();

void run(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void copy_data_to_gpu();

unsigned int setup_shader();
