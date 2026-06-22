#ifndef GOF2_PLATFORM_GL_H
#define GOF2_PLATFORM_GL_H
#include <cstdint>

extern "C" {
uint32_t glCreateShader(uint32_t type);

void glShaderSource(uint32_t shader, uint32_t count, const char **strings, const int *lengths);

void glCompileShader(uint32_t shader);

void glGetShaderiv(uint32_t shader, uint32_t pname, int *params);

void glGetShaderInfoLog(uint32_t shader, uint32_t maxLength, int *length, void *infoLog);

void glDeleteShader(uint32_t shader);

uint32_t glCreateProgram();

void glAttachShader(uint32_t program, uint32_t shader);

void glLinkProgram(uint32_t program);

void glGetProgramiv(uint32_t program, uint32_t pname, int *params);

void glGetProgramInfoLog(uint32_t program, uint32_t maxLength, int *length, void *infoLog);

void glUseProgram(uint32_t program);

void glDeleteProgram(uint32_t program);

int glGetAttribLocation(uint32_t program, const char *name);

int glGetUniformLocation(uint32_t program, const char *name);

void glUniform1i(int location, int v0);

void glUniform1f(int location, float v0);

void glUniform2f(int location, float v0, float v1);

void glUniform3f(int location, float v0, float v1, float v2);

void glUniform4f(int location, float v0, float v1, float v2, float v3);

void glUniform3fv(int location, int count, const float *value);

void glUniform4fv(int location, int count, const float *value);

void glUniformMatrix3fv(int location, int count, uint8_t transpose, const float *value);

void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);

void glVertexAttribPointer(uint32_t index, int size, uint32_t type, uint8_t normalized, int stride,
                           const void *pointer);

void glEnableVertexAttribArray(uint32_t index);

void glDisableVertexAttribArray(uint32_t index);

void glActiveTexture(uint32_t texture);

void glBindBuffer(uint32_t target, uint32_t buffer);

void glBindTexture(uint32_t target, uint32_t texture);

void glBindFramebuffer(uint32_t target, uint32_t framebuffer);

void glBlendFunc(uint32_t sfactor, uint32_t dfactor);

void glClear(uint32_t mask);

void glClearColor(float red, float green, float blue, float alpha);

void glColor(float red, float green, float blue, float alpha);

void glDepthMask(uint8_t flag);

void glEnable(uint32_t cap);

void glDisable(uint32_t cap);

void glViewport(int x, int y, int width, int height);
} // extern "C"

#endif // GOF2_PLATFORM_GL_H
