# PulseShader::SetInActive
elf 0x92554 body 72
Sig: undefined __thiscall SetInActive(PulseShader * this)

## decompile
```c

/* AbyssEngine::PulseShader::SetInActive() */

void __thiscall AbyssEngine::PulseShader::SetInActive(PulseShader *this)

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
  000a2554: push {r4,r6,r7,lr}
  000a2556: add r7,sp,#0x8
  000a2558: mov r4,r0
  000a255a: ldr r0,[r0,#0x20]
  000a255c: cmp r0,#0x0
  000a255e: blt 0x000a2564
  000a2560: blx 0x00070a44
  000a2564: ldr r0,[r4,#0x24]
  000a2566: cmp r0,#0x0
  000a2568: blt 0x000a256e
  000a256a: blx 0x00070a44
  000a256e: ldr r0,[r4,#0x28]
  000a2570: cmp r0,#0x0
  000a2572: blt 0x000a2578
  000a2574: blx 0x00070a44
  000a2578: ldr r0,[r4,#0x2c]
  000a257a: cmp r0,#0x0
  000a257c: blt 0x000a2582
  000a257e: blx 0x00070a44
  000a2582: ldr r0,[r4,#0x30]
  000a2584: cmp r0,#0x0
  000a2586: blt 0x000a2590
  000a2588: pop.w {r4,r6,r7,lr}
  000a258c: b.w 0x001ab778
  000a2590: pop {r4,r6,r7,pc}
  001ab778: bx pc
  001ab77c: ldr r12,[0x1ab784]
  001ab780: add pc,r12,pc
```
