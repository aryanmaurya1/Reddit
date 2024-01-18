#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

enum buffer_type
{
    vbo,
    ebo
};

GLFWwindow *GetWindow();

void run(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

unsigned int setup_vbo_and_cp_data(void *, size_t size);

unsigned int setup_ebo_and_cp_data(void *vertices, size_t size, void *indices, size_t isize);

unsigned int setup_vertex_array_object(enum buffer_type btype);

unsigned int setup_shader();

void draw_triangle(unsigned int VAO, unsigned int shaderProgram, enum buffer_type btype);

void set_wireframe_mode_on();
