# GOF2 matching-decompilation build.
# Runs the exact original toolchain (NDK r18b clang 7.0.2). Build in the OrbStack guest
# where the NDK is installed:  make NDK=/opt/android-ndk-r18b
NDK    ?= /opt/android-ndk-r18b
TC     := $(NDK)/toolchains/llvm/prebuilt/linux-x86_64
CXX    := $(TC)/bin/clang++
# Toolchain archaeology: the target .so was built SIZE-OPTIMIZED (-Oz), not -O2. Evidence:
# across the 376 known-exact functions -Oz keeps every one exact AND flips 16 previously-divergent
# functions to byte-exact (incl. FP InvSqrt + control-flow Player::shoot), where -O2 plateaued at
# 53% mean / 0 of those exact. The earlier "-O2 ceiling" was the wrong opt level. (.comment confirms
# the compiler is exactly NDK r18b clang 7.0.2; arch v7+VFPv3+NEONv1 is the -march=armv7-a default.)
CXXFLAGS := -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fstack-protector -fpic -frtti -Iinclude

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

# Whole-game reassembly: compile EVERY authored function + link a complete libgof2hdaa.so
# (coverage, not byte-exactness — that's the per-function finetune gated by `verify`).
reassemble:
	@bash work/reassemble.sh

coverage:
	@python3 tools/coverage.py

clean:
	rm -rf build/obj build/asm build/libgof2hdaa.so
