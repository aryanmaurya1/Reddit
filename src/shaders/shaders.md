# Shaders

```glsl
#version version_number

in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

void main()
{
    // process input(s) and do some weird graphics stuff
    ...
    ...
    // output processed stuff to output variable
    out_variable_name = weird_stuff_we_processed;
}

```

- Each shader’s entry point is at its main function where we process any input variables and output the results in its output variables.
- In vertex shader each input variable is also known as a vertex attribute.
- OpenGL guarantees there are always at least 16 4-component vertex attributes available.

```glsl
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
```

- GLSL has most of the default basic types we know from languages like C: int, float, double, uint and bool. GLSL also features two container types that we’ll be using a lot, namely vectors and matrices.

- A vector in GLSL is a 1,2,3 or 4 component container for any of the basic types just mentioned above. They can take the following form (n represents the number of components):
• vecn: the default vector of n floats.
• bvecn: a vector of n booleans.
• ivecn: a vector of n integers.
• uvecn: a vector of n unsigned integers.
• dvecn: a vector of n double components.

- we use .x, .y, .z and .w to access their first, second, third and fourth component respectively.
- GLSL also allows you to use 'rgba' for colors or 'stpq' for texture coordinates, accessing the same components.
- vector datatype allows for some interesting and flexible component selection called swizzling.

```glsl
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);
```

## Shader Input and Output

- Vertex shader should receive some form of input otherwise it would be pretty ineffective.
- Fragment shader requires a vec4 color output variable, since the fragment shaders needs to generate a final output color.
- If we want to send data from one shader to the other we’d have to declare an output in the sending shader and a similar input in the receiving shader. 
- When the types and the names are equal on both sides OpenGL will link those variables together and then it is possible to send data between
shaders.

```glsl
#version 330 core
layout (location = 0) in vec3 aPos; // position has attribute position 0
out vec4 vertexColor; // specify a color output to the fragment shader
void main()
{
    gl_Position = vec4(aPos, 1.0); // we give a vec3 to vec4’s constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // output variable to dark-red
}

#version 330 core
out vec4 FragColor;
in vec4 vertexColor; // input variable from vs (same name and type)
void main()
{
    FragColor = vertexColor;
}
```

## Shader Input and Output using Uniforms

- Uniforms are another way to pass data from our application on the CPU to the shaders on the GPU.
- Uniforms are global. Global, meaning that a uniform variable is unique per shader program object, and can be accessed from any shader at any stage in the shader program.
- whatever you set the uniform value to, uniforms will keep their values until they’re either reset or updated.

```
#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // we set this variable in the OpenGL code.
void main()
{
    FragColor = ourColor;
}
```

```c
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```