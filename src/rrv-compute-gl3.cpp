/*
 * Copyright (C) 2015 Claude Heiland-Allen <claude@mathr.co.uk>
 *
 * This file is part of rrv (Radiosity Renderer and Visualizer).
 *
 * rrv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * rrv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rrv.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Triangle.h"
#include "Scene.h"
#include "PatchRandomAccessEnumerator.h"
#include "FormFactorHemicube.h"
#include "FormFactorView.h"

#include <cstdio>

// static allocation of OpenGL texture units
enum TextureID
{
    tidRadiositySrc = 0,
    tidRadiosityDst,
    tidReflectivity,
    tidEmission,
    tidFormFactor,
    tidPatchView,
    tidPatchDepth
};

// debug an OpenGL program
void debug_program(GLuint program) {
  GLint status = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  GLint length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  char *info = 0;
  if (length) {
    info = (char *) malloc(length + 1);
    info[0] = 0;
    glGetProgramInfoLog(program, length, 0, info);
    info[length] = 0;
  }
  if ((info && info[0]) || ! status) {
    fprintf(stderr, "program link info:\n%s", info ? info : "(no info log)");
  }
  if (info) {
    free(info);
  }
}

// debug an OpenGL shader
void debug_shader(GLuint shader, GLenum type, const char *source) {
  GLint status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  char *info = 0;
  if (length) {
    info = (char *) malloc(length + 1);
    info[0] = 0;
    glGetShaderInfoLog(shader, length, 0, info);
    info[length] = 0;
  }
  if ((info && info[0]) || ! status) {
    const char *type_str = "unknown";
    switch (type) {
      case GL_VERTEX_SHADER: type_str = "vertex"; break;
      case GL_FRAGMENT_SHADER: type_str = "fragment"; break;
    }
    fprintf(stderr, "%s shader compile info:\n%s\nshader source:\n%s", type_str, info ? info : "(no info log)", source ? source : "(no source)");
  }
  if (info) {
    free(info);
  }
}

int main(int argc, char **argv)
{
    // check arguments
    if(! (argc > 2))
    {
        std::cerr << "usage: rrv3-compute in.xml out.xml" << std::endl;
        return 1;
    }
    std::string infilename(argv[1]);
    std::string outfilename(argv[2]);

    // Load scene
    Scene scene;
    try
    {
        scene.load(infilename);
    }
    catch( XML::XMLException &e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    scene.divide(1.0);

    PatchRandomAccessEnumerator *patchesPtr = new PatchRandomAccessEnumerator(scene.createPatchSequenceEnumerator());
    PatchRandomAccessEnumerator &patches = *patchesPtr;

    // controls normalization
    int normalizing = 0;

    // controls fidelity of per-patch view rendering
    int quality = 8;
    int edge11 = 1 <<  quality;
    int edge12 = 1 << (quality - 1);
    int edge21 = 2 <<  quality;

    // compute required size of patch data textures
    int patchCount = patches.count();
    int patchBits12 = 0;
    int patchCountMax;
    do
    {
        patchBits12++;
        patchCountMax = 1 << (patchBits12 << 1);
    }
    while (patchBits12 < 16 && ! (patchCount < patchCountMax));
    if(! (patchCount < patchCountMax))
    {
        std::cerr << "too many patches: " << patchCount << std::endl;
        return 1;
    }
    int patchWidth = 1 << patchBits12;
    int patchHeight = 1 << patchBits12;
    std::cerr << "patch data texture size: " << patchWidth << "x" << patchHeight << " (" << patchCountMax << " texels)" << std::endl;

    // create window
    int width  = 1024;
    int height = 512;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(width, height, "rrv3-compute", 0, 0);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); // discard common error from GLEW

    // create GPU patch data
    int bytes = patchCountMax * 3 * 4 * sizeof(GLfloat);
    GLfloat *data = (GLfloat *) calloc(1, bytes);
    for (int i = 0; i < patchCount; ++i)
    {
        Triangle &t = patches[i];
        for(int j=0;j<3;j++)
        {
            int k = 4 * (3 * i + j);
            data[k + 0] = t.vertex[j].x;
            data[k + 1] = t.vertex[j].y;
            data[k + 2] = t.vertex[j].z;
            data[k + 3] = i;
        }
    }
    GLuint vboPatchData;
    glGenBuffers(1, &vboPatchData);
    glBindBuffer(GL_ARRAY_BUFFER, vboPatchData);
    glBufferData(GL_ARRAY_BUFFER, bytes, data, GL_STATIC_DRAW);
    free(data);
    GLuint vaoPatchData;
    glGenVertexArrays(1, &vaoPatchData);
    glBindVertexArray(vaoPatchData);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    float *buf = (float *) calloc(1, patchWidth * patchHeight * 3 * sizeof(float));
    GLuint texPatchData[4];
    glGenTextures(4, &texPatchData[0]);
    glActiveTexture(GL_TEXTURE0 + tidEmission);
    glBindTexture(GL_TEXTURE_2D, texPatchData[tidEmission]);
    for (int i = 0; i < patchCount; ++i)
    {
        buf[3 * i + 0] = patches[i].emission.r;
        buf[3 * i + 1] = patches[i].emission.g;
        buf[3 * i + 2] = patches[i].emission.b;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, patchWidth, patchHeight, 0, GL_RGB, GL_FLOAT, buf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glActiveTexture(GL_TEXTURE0 + tidRadiositySrc);
    glBindTexture(GL_TEXTURE_2D, texPatchData[tidRadiositySrc]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, patchWidth, patchHeight, 0, GL_RGB, GL_FLOAT, buf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glActiveTexture(GL_TEXTURE0 + tidRadiosityDst);
    glBindTexture(GL_TEXTURE_2D, texPatchData[tidRadiosityDst]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, patchWidth, patchHeight, 0, GL_RGB, GL_FLOAT, buf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    for (int i = 0; i < patchCount; ++i)
    {
        buf[3 * i + 0] = patches[i].reflectivity.r;
        buf[3 * i + 1] = patches[i].reflectivity.g;
        buf[3 * i + 2] = patches[i].reflectivity.b;
    }
    glActiveTexture(GL_TEXTURE0 + tidReflectivity);
    glBindTexture(GL_TEXTURE_2D, texPatchData[tidReflectivity]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, patchWidth, patchHeight, 0, GL_RGB, GL_FLOAT, buf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // create GPU form-factor data
    FormFactorHemicube hemicube(edge11, FormFactorHemicube::Cohen1985, true);
    float *bufFormFactor = (float *) calloc(1, edge21 * edge21 * sizeof(float));
    // the hemicube sums to 1
    // for OpenGL mipmapping used later it should have mean 1
    // so multiply by its pixel count
    for (int j = 0; j < edge21; ++j)
        for (int i = 0; i < edge21; ++i)
            bufFormFactor[j * edge21 + i] = hemicube.ff(j, i) * edge21 * edge21;
    glActiveTexture(GL_TEXTURE0 + tidFormFactor);
    GLuint texFormFactor;
    glGenTextures(1, &texFormFactor);
    glBindTexture(GL_TEXTURE_2D, texFormFactor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, edge21, edge21, 0, GL_RED, GL_FLOAT, bufFormFactor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    free(bufFormFactor);

    // create GPU patch view data
    glActiveTexture(GL_TEXTURE0 + tidPatchView);
    GLuint texPatchView;
    glGenTextures(1, &texPatchView);
    glBindTexture(GL_TEXTURE_2D, texPatchView);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, edge21, edge21, 0, GL_RGB, GL_FLOAT, 0);
    glActiveTexture(GL_TEXTURE0 + tidPatchDepth);
    GLuint texPatchDepth;
    glGenTextures(1, &texPatchDepth);
    glBindTexture(GL_TEXTURE_2D, texPatchDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, edge21, edge21, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    GLuint fboPatchView;
    glGenFramebuffers(1, &fboPatchView);
    glBindFramebuffer(GL_FRAMEBUFFER, fboPatchView);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texPatchView, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texPatchDepth, 0);

    // GPU pixel buffer for copying between textures
    GLuint pboRadiosityPixel;
    glGenBuffers(1, &pboRadiosityPixel);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboRadiosityPixel);
    glBufferData(GL_PIXEL_PACK_BUFFER, 4 * sizeof(GLfloat), 0, GL_DYNAMIC_COPY);

    // framebuffer for radiosity normalization
    GLuint fboRadiosityUpdate;
    glGenFramebuffers(1, &fboRadiosityUpdate);
    glBindFramebuffer(GL_FRAMEBUFFER, fboRadiosityUpdate);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texPatchData[tidRadiositySrc], 0);

    // vertex array for radiosity normalization
    GLuint vaoRadiosityUpdate;
    glGenVertexArrays(1, &vaoRadiosityUpdate);

    // per-patch radiosity accumulation shader
    GLuint prgPatchView = glCreateProgram();
    {
        GLuint v = glCreateShader(GL_VERTEX_SHADER);
        const char *vs =
"#version 330 core\n"
"uniform mat4 MVP;\n"
"layout (location = 0) in vec4 vertex;\n"
"out float id;\n"
"void main(void)\n"
"{\n"
"    id = vertex.w;\n"
"    gl_Position = MVP * vec4(vertex.xyz, 1.0);\n"
"}\n"
;
        glShaderSource(v, 1, &vs, 0);
        glCompileShader(v);
        debug_shader(v, GL_VERTEX_SHADER, vs);
        glAttachShader(prgPatchView, v);
        glDeleteShader(v);
        GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fs =
"#version 330 core\n"
"uniform sampler2D radiosity;\n"
"uniform sampler2D formFactor;\n"
"in float id;\n"
"out vec4 colour;\n"
"void main(void)\n"
"{\n"
"    int w = textureSize(radiosity, 0).x;\n"
"    int i = int(id + 0.5);\n"
"    int x = i % w;\n"
"    int y = i / w;\n"
"    vec4 rad = texelFetch(radiosity, ivec2(x, y), 0);\n"
"    float ff = texelFetch(formFactor, ivec2(gl_FragCoord.xy), 0).x;\n"
"    colour = ff * rad;\n"
"}\n"
;
        glShaderSource(f, 1, &fs, 0);
        glCompileShader(f);
        debug_shader(f, GL_FRAGMENT_SHADER, fs);
        glAttachShader(prgPatchView, f);
        glDeleteShader(f);
        glLinkProgram(prgPatchView);
        debug_program(prgPatchView);
        glUseProgram(prgPatchView);
        glUniform1i(glGetUniformLocation(prgPatchView, "radiosity"), tidRadiositySrc);
        glUniform1i(glGetUniformLocation(prgPatchView, "formFactor"), tidFormFactor);
    }
    GLint uniPatchViewMVP = glGetUniformLocation(prgPatchView, "MVP");

    // per-step radiosity normalization shader
    GLuint prgRadiosityUpdate = glCreateProgram();
    {
        GLuint v = glCreateShader(GL_VERTEX_SHADER);
        const char *vs =
"#version 330 core\n"
"void main(void)\n"
"{\n"
"    vec2 p = vec2(1.0, 1.0);\n"
"    switch(gl_VertexID)\n"
"    {\n"
"        case 0: p = vec2(0.0, 0.0); break;\n"
"        case 1: p = vec2(1.0, 0.0); break;\n"
"        case 2: p = vec2(0.0, 1.0); break;\n"
"    }\n"
"    gl_Position = vec4(2.0 * p - vec2(1.0, 1.0), 0.0, 1.0);\n"
"}\n"
;
        glShaderSource(v, 1, &vs, 0);
        glCompileShader(v);
        debug_shader(v, GL_VERTEX_SHADER, vs);
        glAttachShader(prgRadiosityUpdate, v);
        glDeleteShader(v);
        GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fs =
"#version 330 core\n"
"uniform sampler2D radiosity;\n"
"uniform sampler2D reflectivity;\n"
"uniform sampler2D emission;\n"
"uniform float brightness;\n"
"out vec4 colour;\n"
"void main(void)\n"
"{\n"
"    ivec2 texCoord = ivec2(gl_FragCoord.xy);\n"
"    vec4 rad = texelFetch(radiosity, texCoord, 0);\n"
"    vec4 ref = texelFetch(reflectivity, texCoord, 0);\n"
"    vec4 emi = texelFetch(emission, texCoord, 0);\n"
"    colour = max(vec4(0.0), brightness * rad * ref + emi);\n"
"}\n"
;
        glShaderSource(f, 1, &fs, 0);
        glCompileShader(f);
        debug_shader(f, GL_FRAGMENT_SHADER, fs);
        glAttachShader(prgRadiosityUpdate, f);
        glDeleteShader(f);
        glLinkProgram(prgRadiosityUpdate);
        debug_program(prgRadiosityUpdate);
        glUseProgram(prgRadiosityUpdate);
        glUniform1i(glGetUniformLocation(prgRadiosityUpdate, "radiosity"), tidRadiosityDst);
        glUniform1i(glGetUniformLocation(prgRadiosityUpdate, "reflectivity"), tidReflectivity);
        glUniform1i(glGetUniformLocation(prgRadiosityUpdate, "emission"), tidEmission);
    }
    GLint uniRadiosityUpdateBrightness = glGetUniformLocation(prgRadiosityUpdate, "brightness");

    // compute radiosity
    int steps = 32;
    for (int step = 0; step < steps; ++step)
    {
        glfwPollEvents();
        if (glfwWindowShouldClose(window))
            break;

        // compute radiosity received by each patch
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClearDepth(1.0);
        glEnable(GL_SCISSOR_TEST);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboRadiosityPixel);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboRadiosityPixel);
        glBindFramebuffer(GL_FRAMEBUFFER, fboPatchView);
        glBindVertexArray(vaoPatchData);
        glUseProgram(prgPatchView);
        for (int dest = 0; dest < patchCount; ++dest)
        {
            // report progress and check events
            if((dest & 1023) == 0)
            {
                std::cerr << "\rstep " << step << " of " << steps << ", patch " << dest << " of " << patchCount << "      " << std::flush;
                glfwPollEvents();
                if (glfwWindowShouldClose(window))
                    break;
            }
            int e = glGetError();
            if (e)
                std :: cerr << std::endl << "OpenGL error: " << e << std::endl;

            // render scene from point of view of patch
            glScissor(0, 0, edge21, edge21);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            {
                // patch properties
                Triangle &t0 = patches[dest];
                FormFactorView v(t0);

                // in glm form
                glm::vec3 center = glm::vec3(float(v.c.x), float(v.c.y), float(v.c.z));
                glm::vec3 out   = glm::normalize(glm::vec3(float(v.norm.dx), float(v.norm.dy), float(v.norm.dz)));
                glm::vec3 up    = glm::normalize(glm::vec3(glm::vec3(float(v.vctA.dx), float(v.vctA.dy), float(v.vctA.dz))));
                glm::vec3 eye   = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 right = glm::normalize(glm::cross(out, up));
                glm::mat4 projection = glm::perspective(90.0f, 1.0f, 0.001f, 50.0f);
                glm::mat4 model = glm::translate(glm::mat4(), -center);
                glm::mat4 view, mvp;

                // viewport defines the scope of the transform to normalized device coordinates
                // scissor restricts rendering to part of the framebuffer
                // FIXME check that viewports outside the framebuffer are allowed by the spec
                // FIXME it seems to work correctly on my system
                // FIXME OpenGL 4 has GL_VIEWPORT_BOUNDS_MAX for glGetFloatv
                // FIXME which returns 2 values, guarenteed larger than (-32767,32767)
                // forward
                view = glm::lookAt(eye, out, up);
                mvp = projection * view * model;
                glUniformMatrix4fv(uniPatchViewMVP, 1, GL_FALSE, &mvp[0][0]);
                glViewport(edge12, edge12, edge11, edge11);
                glScissor(edge12, edge12, edge11, edge11);
                glDrawArrays(GL_TRIANGLES, 0, 3 * patchCount);
                // right
                view = glm::lookAt(eye, right, up);
                mvp = projection * view * model;
                glUniformMatrix4fv(uniPatchViewMVP, 1, GL_FALSE, &mvp[0][0]);
                glViewport(edge11 + edge12, edge12, edge11, edge11);
                glScissor(edge11 + edge12, edge12, edge12, edge11);
                glDrawArrays(GL_TRIANGLES, 0, 3 * patchCount);
                // left
                view = glm::lookAt(eye, -right, up);
                mvp = projection * view * model;
                glUniformMatrix4fv(uniPatchViewMVP, 1, GL_FALSE, &mvp[0][0]);
                glViewport(-edge11 + edge12, edge12, edge11, edge11);
                glScissor(0, edge12, edge12, edge11);
                glDrawArrays(GL_TRIANGLES, 0, 3 * patchCount);
                // up
                view = glm::lookAt(eye, up, -out);
                mvp = projection * view * model;
                glUniformMatrix4fv(uniPatchViewMVP, 1, GL_FALSE, &mvp[0][0]);
                glViewport(edge12, edge11 + edge12, edge11, edge11);
                glScissor(edge12, edge11 + edge12, edge11, edge12);
                glDrawArrays(GL_TRIANGLES, 0, 3 * patchCount);
                // down
                view = glm::lookAt(eye, -up, out);
                mvp = projection * view * model;
                glUniformMatrix4fv(uniPatchViewMVP, 1, GL_FALSE, &mvp[0][0]);
                glViewport(edge12, -edge11 + edge12, edge11, edge11);
                glScissor(edge12, 0, edge11, edge12);
                glDrawArrays(GL_TRIANGLES, 0, 3 * patchCount);
            }

            // periodically update display
            // updating every step causes stalls on vsync
            if((dest & 1023) == 0)
            {
                glScissor(0, 0, width / 2, height);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                glBlitFramebuffer(0, 0, edge21, edge21, 0, 0, width / 2, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
                glBindFramebuffer(GL_FRAMEBUFFER, fboPatchView);
                glfwSwapBuffers(window);
            }

            // reduce to single texel
            glActiveTexture(GL_TEXTURE0 + tidPatchView);
            glGenerateMipmap(GL_TEXTURE_2D);

            // copy to destination via pack/unpack buffer
            glGetTexImage(GL_TEXTURE_2D, quality + 1, GL_RGB, GL_FLOAT, 0);
            glActiveTexture(GL_TEXTURE0 + tidRadiosityDst);
            int x = dest % patchWidth;
            int y = dest / patchWidth;
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGB, GL_FLOAT, 0);

        } // for dest
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);

        float peak = 1;
        if(normalizing)
        {
            // read back to CPU and compute peak for normalization
            glActiveTexture(GL_TEXTURE0 + tidRadiosityDst);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, buf);
            peak = 0;
            for (int i = 0; i < 3 * patchCount; ++i)
                peak = fmaxf(peak, buf[i]);
        }

        // normalize radiosity and update by reflectivity and emission
        glBindFramebuffer(GL_FRAMEBUFFER, fboRadiosityUpdate);
        glBindVertexArray(vaoRadiosityUpdate);
        glUseProgram(prgRadiosityUpdate);
        glUniform1f(uniRadiosityUpdateBrightness, 1 / peak);
        glViewport(0, 0, patchWidth, patchHeight);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // display raw updated radiosity map
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, patchWidth, patchHeight, width / 2, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    } // for step

    // read back results and save updated scene
    glActiveTexture(GL_TEXTURE0 + tidRadiositySrc);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, buf);
    for (int i = 0; i < patchCount; ++i)
    {
        patches[i].radiosity.r = buf[3 * i + 0];
        patches[i].radiosity.g = buf[3 * i + 1];
        patches[i].radiosity.b = buf[3 * i + 2];
    }
    free(buf);
    scene.save(outfilename);

    return 0;
}
