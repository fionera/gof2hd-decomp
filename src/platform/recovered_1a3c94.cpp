// Recovered C++ ABI global allocation operators from the Android binary
// (ghidra_addr 0x1a3c94..0x1a3d24). The game ships its own copies of the four
// replaceable global new/delete operators rather than pulling them from the
// shared libc++abi, so they appear as defined symbols in libgof2hdaa.so:
//
//   _Znwj   operator new(unsigned int)              0x1a3c94  (full body)
//   _Znaj   operator new[](unsigned int)            0x1a3cf2  -> operator new
//   _ZdlPv  operator delete(void*)                  0x1a3d10  -> free
//   _ZdlPvj operator delete(void*, unsigned int)    0x1a3d18  -> operator delete
//   _ZdaPv  operator delete[](void*)                0x1a3d1c  -> operator delete
//   _ZdaPvj operator delete[](void*, unsigned int)  0x1a3d24  -> operator delete[]
//
// This is the canonical libsupc++ implementation: operator new loops calling the
// installed new-handler until malloc() succeeds, throwing std::bad_alloc once the
// handler has been cleared. The sized and array forms delegate, exactly as the
// authoritative disassembly's tail-call thunks (b.w) do. size_t is 32-bit on
// armeabi-v7a, so the unsigned-int parameter mangles to `j`, matching the binary.

#include <cstdlib>
#include <new>

// operator new(unsigned int) -- 0x1a3c94 (_Znwj)
void *operator new(std::size_t size)
{
    // A request for zero bytes still yields a unique, freeable pointer.
    if (size == 0)
        size = 1;

    void *p;
    while ((p = std::malloc(size)) == nullptr) {
        std::new_handler handler = std::get_new_handler();
        if (handler == nullptr)
            throw std::bad_alloc();
        handler();
    }
    return p;
}

// operator new[](unsigned int) -- 0x1a3cf2 (_Znaj)
void *operator new[](std::size_t size)
{
    return ::operator new(size);
}

// operator delete(void*) -- 0x1a3d10 (_ZdlPv)
// The scalar deallocation operator: a null pointer is a no-op, otherwise the
// block is returned to the C heap. Every other delete form funnels through here.
void operator delete(void *ptr) noexcept
{
    if (ptr != nullptr)
        std::free(ptr);
}

// operator delete(void*, unsigned int) -- 0x1a3d18 (_ZdlPvj)
// Sized delete: the size hint is unused, the request delegates to the unsized form.
void operator delete(void *ptr, std::size_t) noexcept
{
    ::operator delete(ptr);
}

// operator delete[](void*) -- 0x1a3d1c (_ZdaPv)
void operator delete[](void *ptr) noexcept
{
    ::operator delete(ptr);
}

// operator delete[](void*, unsigned int) -- 0x1a3d24 (_ZdaPvj)
void operator delete[](void *ptr, std::size_t) noexcept
{
    ::operator delete[](ptr);
}
