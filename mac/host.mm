// Galaxy on Fire 2 — native macOS host.
// Opens a Cocoa window + OpenGL context and drives the decompiled engine through its JNI entry
// points. The game is loaded dynamically (dlopen) so this host builds and runs standalone today;
// once libgof2.dylib (the engine built for arm64-macos) is present next to it, the host bootstraps
// the engine: JNI_OnLoad -> ndk23_setRootDirectory/ZipDirectory -> ToJNI_initialize -> per-frame
// ToJNI_renderstep. Build: bash mac/build.sh ; run: mac/gof2_host
#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>

// ---- engine entry points (resolved from libgof2.dylib at runtime) ------------
typedef void (*fn_void)();
typedef void (*fn_jni4)(void*, void*, int, int);   // (JNIEnv*, jobject, w/h or zip handle, ...)
typedef void (*fn_setdir)(const char*);
static void*    g_game       = nullptr;
static fn_setdir setRoot     = nullptr;
static fn_setdir setZip      = nullptr;
static fn_jni4  toInitialize = nullptr;
static fn_jni4  toRenderstep = nullptr;
static fn_jni4  toResize     = nullptr;
static int      g_w = 960, g_h = 640;
static bool     g_booted = false;

static void load_game()
{
    const char* path = "libgof2.dylib";
    g_game = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!g_game) { fprintf(stderr, "[host] no %s yet (%s) — running standalone\n", path, dlerror()); return; }
    setRoot      = (fn_setdir)dlsym(g_game, "ndk23_setRootDirectory");
    setZip       = (fn_setdir)dlsym(g_game, "ndk23_setZipDirectory");
    toInitialize = (fn_jni4)  dlsym(g_game, "Java_net_fishlabs_gof2hdallandroid2012_ToJNI_initialize");
    toRenderstep = (fn_jni4)  dlsym(g_game, "Java_net_fishlabs_gof2hdallandroid2012_ToJNI_renderstep");
    toResize     = (fn_jni4)  dlsym(g_game, "Java_net_fishlabs_gof2hdallandroid2012_ToJNI_resize");
    fprintf(stderr, "[host] loaded %s (init=%p render=%p)\n", path, (void*)toInitialize, (void*)toRenderstep);
}

static void boot_game()
{
    if (g_booted || !g_game) return;
    const char* root = getenv("GOF2_DATA"); if (!root) root = "./data";
    if (setRoot) setRoot(root);
    if (setZip)  setZip(root);
    if (toInitialize) toInitialize(nullptr, nullptr, g_w, g_h);   // JNIEnv/obj are shimmed inside the engine
    if (toResize)     toResize(nullptr, nullptr, g_w, g_h);
    g_booted = true;
}

// ---- OpenGL view -------------------------------------------------------------
@interface GofView : NSOpenGLView
@end
@implementation GofView
- (void)prepareOpenGL { [super prepareOpenGL]; load_game(); }
- (void)drawRect:(NSRect)r {
    [[self openGLContext] makeCurrentContext];
    if (g_game) {
        boot_game();
        if (toRenderstep) toRenderstep(nullptr, nullptr, g_w, g_h);
    } else {
        glClearColor(0.05f, 0.08f, 0.12f, 1.0f);   // standalone: dark-blue clear so we can see it's alive
        glClear(GL_COLOR_BUFFER_BIT);
    }
    [[self openGLContext] flushBuffer];
}
@end

// ---- app ---------------------------------------------------------------------
int main(int argc, const char** argv)
{
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        NSRect frame = NSMakeRect(0, 0, g_w, g_h);
        NSWindow* win = [[NSWindow alloc] initWithContentRect:frame
            styleMask:(NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskResizable)
            backing:NSBackingStoreBuffered defer:NO];
        [win setTitle:@"Galaxy on Fire 2 — native host"];
        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFADoubleBuffer, NSOpenGLPFAColorSize, 24, NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy, 0 };
        NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        GofView* view = [[GofView alloc] initWithFrame:frame pixelFormat:pf];
        [win setContentView:view];
        [win center]; [win makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];
        // ~60fps redraw loop
        [NSTimer scheduledTimerWithTimeInterval:1.0/60.0 repeats:YES block:^(NSTimer*){ [view setNeedsDisplay:YES]; }];
        [NSApp run];
    }
    return 0;
}
