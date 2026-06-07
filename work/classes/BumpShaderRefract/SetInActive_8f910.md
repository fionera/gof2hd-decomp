# BumpShaderRefract::SetInActive
elf 0x8f910 body 42
Sig: undefined __thiscall SetInActive(BumpShaderRefract * this)

## decompile
```c

/* AbyssEngine::BumpShaderRefract::SetInActive() */

void __thiscall AbyssEngine::BumpShaderRefract::SetInActive(BumpShaderRefract *this)

{
  if (-1 < *(int *)(this + 0x20)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glDisableVertexAttribArray();
  }
  if (*(int *)(this + 0x28) < 0) {
    return;
  }
  (*(code *)(DAT_001ab784 + 0x1ab788))();
  return;
}

```

## target disasm
```
  0009f910: push {r4,r6,r7,lr}
  0009f912: add r7,sp,#0x8
  0009f914: mov r4,r0
  0009f916: ldr r0,[r0,#0x20]
  0009f918: cmp r0,#0x0
  0009f91a: blt 0x0009f920
  0009f91c: blx 0x00070a44
  0009f920: ldr r0,[r4,#0x24]
  0009f922: cmp r0,#0x0
  0009f924: blt 0x0009f92a
  0009f926: blx 0x00070a44
  0009f92a: ldr r0,[r4,#0x28]
  0009f92c: cmp r0,#0x0
  0009f92e: blt 0x0009f938
  0009f930: pop.w {r4,r6,r7,lr}
  0009f934: b.w 0x001ab778
  0009f938: pop {r4,r6,r7,pc}
```
