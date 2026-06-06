# GOF2 matching-decompilation build.
# Runs the exact original toolchain (NDK r18b clang 7.0.2). Build in the OrbStack guest
# where the NDK is installed:  make NDK=/opt/android-ndk-r18b
NDK    ?= /opt/android-ndk-r18b
TC     := $(NDK)/toolchains/llvm/prebuilt/linux-x86_64
CXX    := $(TC)/bin/clang++
# Verified flags that reproduced ArrayAdd byte-exactly (keep the frame pointer; -O2).
CXXFLAGS := -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -O2 -fpic -frtti -Iinclude

SRCS := $(shell find src -name '*.cpp' 2>/dev/null)
OBJS := $(patsubst src/%.cpp,build/obj/%.o,$(SRCS))
ASMS := $(patsubst src/%.cpp,build/asm/%.s,$(SRCS))

.PHONY: all asm clean
all: $(OBJS)
asm: $(ASMS)

build/obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/asm/%.s: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -S $< -o $@

# Link recovered objects into the shared library. Mirrors the target's NEEDED libs + soname.
# (Full byte-match relink also needs the original link order, version script and build-id —
#  see docs/RELINK.md. Until the platform stub libs are present, link with undefined allowed.)
NEEDED := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lfmodex -lfmodevent -lm -ldl
LDFLAGS := -shared -fuse-ld=lld -Wl,-soname,libgof2hdaa.so -Wl,--build-id \
           -Wl,--allow-shlib-undefined -nostdlib
build/libgof2hdaa.so: $(OBJS)
	$(CXX) -target armv7-none-linux-androideabi16 $(LDFLAGS) $(OBJS) -o $@
link: build/libgof2hdaa.so

verify:
	@bash tools/verify.sh

coverage:
	@python3 tools/coverage.py

clean:
	rm -rf build/obj build/asm build/libgof2hdaa.so
