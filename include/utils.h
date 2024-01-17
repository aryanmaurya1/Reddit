#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

GLFWwindow *GetWindow();

void run(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

unsigned int setup_vbo_and_cp_data(void *, size_t size);

unsigned int setup_vertex_array_object();

unsigned int setup_shader();

void draw_triangle(unsigned int, unsigned int);