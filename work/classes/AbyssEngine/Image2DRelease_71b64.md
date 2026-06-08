# AbyssEngine::Image2DRelease
elf 0x71b64 body 28
Sig: undefined __stdcall Image2DRelease(Engine * param_1, Image2D * * param_2)

## decompile
```c

/* AbyssEngine::Image2DRelease(AbyssEngine::Engine*, AbyssEngine::Image2D**) */

void AbyssEngine::Image2DRelease(Engine *param_1,Image2D **param_2)

{
  if (*param_2 != (Image2D *)0x0) {
    MeshRelease(param_1,(Mesh **)*param_2);
    if (*param_2 != (Image2D *)0x0) {
      operator_delete(*param_2);
    }
    *param_2 = (Image2D *)0x0;
  }
  return;
}

```

## target disasm
```
  00081b64: push {r4,r6,r7,lr}
  00081b66: add r7,sp,#0x8
  00081b68: mov r4,r1
  00081b6a: ldr r1,[r1,#0x0]
  00081b6c: cbz r1,0x00081b7e
  00081b6e: blx 0x0006ebd8
  00081b72: ldr r0,[r4,#0x0]
  00081b74: cbz r0,0x00081b7a
  00081b76: blx 0x0006eb48
  00081b7a: movs r0,#0x0
  00081b7c: str r0,[r4,#0x0]
  00081b7e: pop {r4,r6,r7,pc}
```
