# BumpRimCubeShader::SetInActive
elf 0x8fdcc body 62
Sig: undefined __thiscall SetInActive(BumpRimCubeShader * this)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader::SetInActive() */

void __thiscall AbyssEngine::BumpRimCubeShader::SetInActive(BumpRimCubeShader *this)

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
  0009fdcc: push {r4,r6,r7,lr}
  0009fdce: add r7,sp,#0x8
  0009fdd0: mov r4,r0
  0009fdd2: ldr r0,[r0,#0x20]
  0009fdd4: cmp r0,#0x0
  0009fdd6: blt 0x0009fddc
  0009fdd8: blx 0x00070a44
  0009fddc: ldr r0,[r4,#0x24]
  0009fdde: cmp r0,#0x0
  0009fde0: blt 0x0009fde6
  0009fde2: blx 0x00070a44
  0009fde6: ldr r0,[r4,#0x28]
  0009fde8: cmp r0,#0x0
  0009fdea: blt 0x0009fdf0
  0009fdec: blx 0x00070a44
  0009fdf0: ldr r0,[r4,#0x2c]
  0009fdf2: cmp r0,#0x0
  0009fdf4: blt 0x0009fdfa
  0009fdf6: blx 0x00070a44
  0009fdfa: ldr r0,[r4,#0x30]
  0009fdfc: cmp r0,#0x0
  0009fdfe: blt 0x0009fe08
  0009fe00: pop.w {r4,r6,r7,lr}
  0009fe04: b.w 0x001ab778
  0009fe08: pop {r4,r6,r7,pc}
```
