// host/main.cpp — entry point for the macOS native port of Galaxy on Fire 2 (Milestone 2).
//
// Placeholder. The real boot path — SDL2/GLFW window, GLES2->OpenGL (or ANGLE) backend, input,
// asset mounting (.aei/.bin/shaders/sounds), and engine/application init — lands with the platform
// layer (ported from the gof2-vita FalsoJNI/AFakeNative shims). For now this just lets the project
// build and run a trivial target so CLion has a runnable configuration.
#include <cstdio>

int main(int /*argc*/, char** /*argv*/) {
    std::printf("Galaxy on Fire 2 - native port (work in progress)\n");
    return 0;
}
