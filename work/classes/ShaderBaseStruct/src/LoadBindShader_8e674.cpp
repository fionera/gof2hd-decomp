#include "class.h"

namespace AbyssEngine {

uint32_t ShaderBaseStruct::LoadBindShader(const char *vertexPath, const char *fragmentPath)
{
    volatile uint32_t stackGuard = __stack_chk_guard;
    register const char *fragment = fragmentPath;
    uint32_t result = 0;

    if (vertexPath != 0 && fragment != 0) {
        shader_vertex_path(this) = vertexPath;
        shader_fragment_path(this) = fragment;

        uint32_t handle;
        if (AEFile::OpenRead(vertexPath, &handle) != 0) {
            uint32_t vertexSize = AEFile::GetFileSize(handle);
            uint32_t allocSize = vertexSize + 1;
            if ((int)vertexSize < -1) {
                allocSize = 0xffffffff;
            }
            char *vertexSource = (char *)operator new[](allocSize);
            AEFile::Read(vertexSize, vertexSource, handle);
            AEFile::Close(handle);
            vertexSource[vertexSize] = 0;

            if (AEFile::OpenRead(fragment, &handle) != 0) {
                uint32_t fragmentSize = AEFile::GetFileSize(handle);
                allocSize = fragmentSize + 1;
                if ((int)fragmentSize < -1) {
                    allocSize = 0xffffffff;
                }
                char *fragmentSource = (char *)operator new[](allocSize);
                AEFile::Read(fragmentSize, fragmentSource, handle);
                AEFile::Close(handle);
                fragmentSource[fragmentSize] = 0;

                result = ES2LoadProgram(vertexSource, fragmentSource);
                operator delete[](fragmentSource);
                operator delete[](vertexSource);
            }
        }
    }

    uint32_t savedGuard = stackGuard;
    uint32_t currentGuard = *(volatile uint32_t *)&__stack_chk_guard;
    uint32_t stackDifference = currentGuard - savedGuard;
    if (stackDifference == 0) {
        return result;
    }
    __stack_chk_fail(stackDifference);
}

} // namespace AbyssEngine
