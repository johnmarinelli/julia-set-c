#ifndef JOHN_SHADERS_H
#define JOHN_SHADERS_H

namespace julia {

  namespace shaders {
    static const char *  fragment_shader_src =
    "// Julia set renderer - Fragment Shader                                                \n"
    "// Graham Sellers                                                                      \n"
    "// OpenGL SuperBible                                                                   \n"
    "#version 150 core                                                                      \n"
    "                                                                                       \n"
    "in vec2 initial_z;                                                                     \n"
    "                                                                                       \n"
    "out vec4 color;                                                                        \n"
    "                                                                                       \n"
    "uniform sampler1D tex_gradient;                                                        \n"
    "uniform vec2 C;                                                                        \n"
    "                                                                                       \n"
    "void main(void)                                                                        \n"
    "{                                                                                      \n"
    "    vec2 Z = initial_z;                                                                \n"
    "    int iterations = 0;                                                                \n"
    "    const float threshold_squared = 16.0;\n"
    "    const int max_iterations = 512;\n"
    "    while (iterations < max_iterations && dot(Z, Z) < threshold_squared) {             \n"
    "        vec2 Z_squared;                                                                \n"
    "        Z_squared.x = Z.x * Z.x - Z.y * Z.y;                                           \n"
    "        Z_squared.y = 2.0 * Z.x * Z.y;                                                 \n"
    "        Z = Z_squared + C;                                                             \n"
    "        iterations++;                                                                  \n"
    "    }                                                                                  \n"
    "    if (iterations == max_iterations)                                                  \n"
    "        color = vec4(0.0, 0.0, 0.0, 1.0);                                              \n"
    "    else                                                                               \n"
    "        color = texture(tex_gradient, float(iterations) / float(max_iterations));      \n"
    "    //color = vec4(1.0, 0.5, 0.25, 1.0);      \n"
    "}                                                                                      \n";

    static const char * vertex_shader_src  =
    "// Julia set renderer - Vertex Shader                                                  \n"
    "// Graham Sellers                                                                      \n"
    "// OpenGL SuperBible                                                                   \n"
    "#version 150 core                                                                      \n"
    "                                                                                       \n"
    "// Zoom factor                                                                         \n"
    "uniform float zoom;                                                                    \n"
    "                                                                                       \n"
    "// Offset vector                                                                       \n"
    "uniform vec2 offset;                                                                   \n"
    "                                                                                       \n"
    "out vec2 initial_z;                                                                    \n"
    "                                                                                       \n"
    "void main(void)                                                                        \n"
    "{                                                                                      \n"
    "    const vec4 vertices[4] = vec4[4](vec4(-1.0, -1.0, 0.0, 1.0),                       \n"
    "                                     vec4( 1.0, -1.0, 0.0, 1.0),                       \n"
    "                                     vec4( 1.0,  1.0, 0.0, 1.0),                       \n"
    "                                     vec4(-1.0,  1.0, 0.0, 1.0));                      \n"
    "    initial_z = (vertices[gl_VertexID].xy * zoom) + offset;                            \n"
    "    gl_Position = vertices[gl_VertexID];                                               \n"
    "}                                                                                      \n";

  }

}

#endif
