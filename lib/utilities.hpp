#ifndef JOHN_UTILITIES_H
#define JOHN_UTILITIES_H

namespace john 
{

namespace utilities
{

  int compile_shaders(const char* vtx_shdr_txt, const char* frg_shdr_txt) {
    GLuint vtx_shader, frg_shader, programID;
    vtx_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtx_shader, 1,&vtx_shdr_txt, NULL);
    glCompileShader(vtx_shader);

    frg_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frg_shader, 1, &frg_shdr_txt, NULL);
    glCompileShader(frg_shader);

    programID = glCreateProgram();
    glAttachShader(programID, vtx_shader);
    glAttachShader(programID, frg_shader);
    glLinkProgram(programID);

    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);

    return programID;
  }

  bool write_png_file(char* filename, float* pixels, uint32_t width, uint32_t height) 
  {
    uint32_t x, y, ctr;
    pngwriter png(width, height, 0, filename);
    printf("Width: %d Height: %d\n", width, height);

    ctr = 0;

    for (y = 0; y < height; ++y) { 
      for (x = 0; x < width; ++x) { 
        float r, g, b;
        uint32_t y_off = y * width;
        uint32_t off = y > 0 ? x * 3 - y :  x * 3;

        r = pixels[ctr];
        g = pixels[ctr + 1];
        b = pixels[ctr + 2];

        png.plot(x, y, r, g, b);
        ctr += 3;
      }
    }

    png.close();
    printf("Finished writing png.\n");
    return true;
  }

}

}

#endif
