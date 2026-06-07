# Engine::SetPerspMatrix
elf 0x85478 body 108
Sig: undefined __thiscall SetPerspMatrix(Engine * this, float * param_1)

## decompile
```c

/* AbyssEngine::Engine::SetPerspMatrix(float*) */

void __thiscall AbyssEngine::Engine::SetPerspMatrix(Engine *this,float *param_1)

{
  if (**(char **)(DAT_000954e4 + 0x9547e) != '\0') {
    *(float *)(this + 900) = *param_1;
    *(float *)(this + 0x388) = param_1[1];
    *(float *)(this + 0x38c) = param_1[2];
    *(float *)(this + 0x390) = param_1[3];
    *(float *)(this + 0x394) = param_1[4];
    *(float *)(this + 0x398) = param_1[5];
    *(float *)(this + 0x39c) = param_1[6];
    *(float *)(this + 0x3a0) = param_1[7];
    *(float *)(this + 0x3a4) = param_1[8];
    *(float *)(this + 0x3a8) = param_1[9];
    *(float *)(this + 0x3ac) = param_1[10];
    *(float *)(this + 0x3b0) = param_1[0xb];
    *(float *)(this + 0x3b4) = param_1[0xc];
    *(float *)(this + 0x3b8) = param_1[0xd];
    *(float *)(this + 0x3bc) = param_1[0xe];
    *(float *)(this + 0x3c0) = param_1[0xf];
  }
  return;
}

```

## target disasm
```
  00095478: ldr r2,[0x000954e4]
  0009547a: add r2,pc
  0009547c: ldr r2,[r2,#0x0]
  0009547e: ldrb r2,[r2,#0x0]
  00095480: cbz r2,0x000954e2
  00095482: ldr r2,[r1,#0x0]
  00095484: str.w r2,[r0,#0x384]
  00095488: ldr r2,[r1,#0x4]
  0009548a: str.w r2,[r0,#0x388]
  0009548e: ldr r2,[r1,#0x8]
  00095490: str.w r2,[r0,#0x38c]
  00095494: ldr r2,[r1,#0xc]
  00095496: str.w r2,[r0,#0x390]
  0009549a: ldr r2,[r1,#0x10]
  0009549c: str.w r2,[r0,#0x394]
  000954a0: ldr r2,[r1,#0x14]
  000954a2: str.w r2,[r0,#0x398]
  000954a6: ldr r2,[r1,#0x18]
  000954a8: str.w r2,[r0,#0x39c]
  000954ac: ldr r2,[r1,#0x1c]
  000954ae: str.w r2,[r0,#0x3a0]
  000954b2: ldr r2,[r1,#0x20]
  000954b4: str.w r2,[r0,#0x3a4]
  000954b8: ldr r2,[r1,#0x24]
  000954ba: str.w r2,[r0,#0x3a8]
  000954be: ldr r2,[r1,#0x28]
  000954c0: str.w r2,[r0,#0x3ac]
  000954c4: ldr r2,[r1,#0x2c]
  000954c6: str.w r2,[r0,#0x3b0]
  000954ca: ldr r2,[r1,#0x30]
  000954cc: str.w r2,[r0,#0x3b4]
  000954d0: ldr r2,[r1,#0x34]
  000954d2: str.w r2,[r0,#0x3b8]
  000954d6: ldr r2,[r1,#0x38]
  000954d8: str.w r2,[r0,#0x3bc]
  000954dc: ldr r1,[r1,#0x3c]
  000954de: str.w r1,[r0,#0x3c0]
  000954e2: bx lr
```
