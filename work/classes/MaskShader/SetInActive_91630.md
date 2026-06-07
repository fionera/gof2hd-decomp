# MaskShader::SetInActive
elf 0x91630 body 42
Sig: undefined __thiscall SetInActive(MaskShader * this)

## decompile
```c

/* AbyssEngine::MaskShader::SetInActive() */

void __thiscall AbyssEngine::MaskShader::SetInActive(MaskShader *this)

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
  000a1630: push {r4,r6,r7,lr}
  000a1632: add r7,sp,#0x8
  000a1634: mov r4,r0
  000a1636: ldr r0,[r0,#0x20]
  000a1638: cmp r0,#0x0
  000a163a: blt 0x000a1640
  000a163c: blx 0x00070a44
  000a1640: ldr r0,[r4,#0x24]
  000a1642: cmp r0,#0x0
  000a1644: blt 0x000a164a
  000a1646: blx 0x00070a44
  000a164a: ldr r0,[r4,#0x28]
  000a164c: cmp r0,#0x0
  000a164e: blt 0x000a1658
  000a1650: pop.w {r4,r6,r7,lr}
  000a1654: b.w 0x001ab778
  000a1658: pop {r4,r6,r7,pc}
```
