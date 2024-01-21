#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

float vertices[] = {
    -0.5f,
    -0.5f,
    0.0f,
    0.5f,
    -0.5f,
    0.0f,
    0.0f,
    0.5f,
    0.0f,
};

float ebo_vertices[] = {
    0.5f, 0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f   // top left
};

unsigned int ebo_indices[] = {
    // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

GLFWwindow *GetWindow()
{
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return NULL;
    }
    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void run(GLFWwindow *window)
{
    unsigned int VAO = setup_vertex_array_object(ebo);
    unsigned int shaderProgram = setup_shader();

    set_wireframe_mode_on();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_triangle(VAO, shaderProgram, vbo);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int setup_vbo_and_cp_data(void *vertices, size_t size)
{
    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return VBO;
}

unsigned int setup_ebo_and_cp_data(void *vertices, size_t size, void *indices, size_t isize)
{
    setup_vbo_and_cp_data(vertices, size);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices, GL_STATIC_DRAW);
}

unsigned int setup_vertex_array_object(enum buffer_type btype)
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    if (btype == vbo)
    {
        setup_vbo_and_cp_data(vertices, sizeof(vertices));
    }
    else if (btype == ebo)
    {
        setup_ebo_and_cp_data(ebo_vertices, sizeof(ebo_vertices), ebo_indices, sizeof(ebo_indices));
    }

    return VAO;
}

unsigned int setup_shader()
{
    int success;
    char infoLog[512];

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n,%s", infoLog);
        return 0;
    }

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor\n;"
                                       "void main()\n"
                                       "{\n"
                                       "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n,%s", infoLog);
        return 0;
    }

    unsigned int dynamicColorShaderId = set_dynamic_color_shader();

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, dynamicColorShaderId);
    // glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINKING_FAILED\n,%s", infoLog);
        return 0;
    }
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void set_wireframe_mode_on()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void draw_triangle(unsigned int VAO, unsigned int shaderProgram, enum buffer_type btype)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int uniformLocation = glGetUniformLocation(shaderProgram, "dynamicColor");
    glUniform4f(uniformLocation, 0.0f, greenValue, 0.0f, 1.0f);

    if (btype == vbo)
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    else if (btype == ebo)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

unsigned int set_dynamic_color_shader()
{
    int success;
    char infoLog[520];

    const char *shaderSource = "#version 330 core\n \
    out vec4 FragColor;\n \
    uniform vec4 dynamicColor; // we set this variable in the OpenGL code.\n \
    void main()\n \
    { \
        FragColor = dynamicColor;\n \
    }\n \
    ";

    unsigned int dynamicColorShaderId;
    dynamicColorShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(dynamicColorShaderId, 1, &shaderSource, NULL);
    glCompileShader(dynamicColorShaderId);

    glGetShaderiv(dynamicColorShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(dynamicColorShaderId, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n,%s", infoLog);
        return 0;
    }
    return dynamicColorShaderId;
}
