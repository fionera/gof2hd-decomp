# BumpShaderCloak::SetInActive
elf 0x8ae98 body 62
Sig: undefined __thiscall SetInActive(BumpShaderCloak * this)

## decompile
```c

/* AbyssEngine::BumpShaderCloak::SetInActive() */

void __thiscall AbyssEngine::BumpShaderCloak::SetInActive(BumpShaderCloak *this)

{
  if (-1 < *(int *)(this + 0x20)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x28)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x2c)) {
    glDisableVertexAttribArray();
  }
  if (*(int *)(this + 0x30) < 0) {
    return;
  }
  (*(code *)(DAT_001ab784 + 0x1ab788))();
  return;
}

```

## target disasm
```
  0009ae98: push {r4,r6,r7,lr}
  0009ae9a: add r7,sp,#0x8
  0009ae9c: mov r4,r0
  0009ae9e: ldr r0,[r0,#0x20]
  0009aea0: cmp r0,#0x0
  0009aea2: blt 0x0009aea8
  0009aea4: blx 0x00070a44
  0009aea8: ldr r0,[r4,#0x24]
  0009aeaa: cmp r0,#0x0
  0009aeac: blt 0x0009aeb2
  0009aeae: blx 0x00070a44
  0009aeb2: ldr r0,[r4,#0x28]
  0009aeb4: cmp r0,#0x0
  0009aeb6: blt 0x0009aebc
  0009aeb8: blx 0x00070a44
  0009aebc: ldr r0,[r4,#0x2c]
  0009aebe: cmp r0,#0x0
  0009aec0: blt 0x0009aec6
  0009aec2: blx 0x00070a44
  0009aec6: ldr r0,[r4,#0x30]
  0009aec8: cmp r0,#0x0
  0009aeca: blt 0x0009aed4
  0009aecc: pop.w {r4,r6,r7,lr}
  0009aed0: b.w 0x001ab778
  0009aed4: pop {r4,r6,r7,pc}
```
