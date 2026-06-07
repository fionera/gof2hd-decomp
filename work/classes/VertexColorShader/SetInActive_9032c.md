# VertexColorShader::SetInActive
elf 0x9032c body 72
Sig: undefined __thiscall SetInActive(VertexColorShader * this)

## decompile
```c

/* AbyssEngine::VertexColorShader::SetInActive() */

void __thiscall AbyssEngine::VertexColorShader::SetInActive(VertexColorShader *this)

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
  if (-1 < *(int *)(this + 0x30)) {
    glDisableVertexAttribArray();
  }
  if (*(int *)(this + 0x34) < 0) {
    return;
  }
  (*(code *)(DAT_001ab784 + 0x1ab788))();
  return;
}

```

## target disasm
```
  000a032c: push {r4,r6,r7,lr}
  000a032e: add r7,sp,#0x8
  000a0330: mov r4,r0
  000a0332: ldr r0,[r0,#0x20]
  000a0334: cmp r0,#0x0
  000a0336: blt 0x000a033c
  000a0338: blx 0x00070a44
  000a033c: ldr r0,[r4,#0x24]
  000a033e: cmp r0,#0x0
  000a0340: blt 0x000a0346
  000a0342: blx 0x00070a44
  000a0346: ldr r0,[r4,#0x28]
  000a0348: cmp r0,#0x0
  000a034a: blt 0x000a0350
  000a034c: blx 0x00070a44
  000a0350: ldr r0,[r4,#0x2c]
  000a0352: cmp r0,#0x0
  000a0354: blt 0x000a035a
  000a0356: blx 0x00070a44
  000a035a: ldr r0,[r4,#0x30]
  000a035c: cmp r0,#0x0
  000a035e: blt 0x000a0364
  000a0360: blx 0x00070a44
  000a0364: ldr r0,[r4,#0x34]
  000a0366: cmp r0,#0x0
  000a0368: blt 0x000a0372
  000a036a: pop.w {r4,r6,r7,lr}
  000a036e: b.w 0x001ab778
  000a0372: pop {r4,r6,r7,pc}
```
