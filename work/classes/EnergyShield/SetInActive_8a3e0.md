# EnergyShield::SetInActive
elf 0x8a3e0 body 42
Sig: undefined __thiscall SetInActive(EnergyShield * this)

## decompile
```c

/* AbyssEngine::EnergyShield::SetInActive() */

void __thiscall AbyssEngine::EnergyShield::SetInActive(EnergyShield *this)

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
  0009a3e0: push {r4,r6,r7,lr}
  0009a3e2: add r7,sp,#0x8
  0009a3e4: mov r4,r0
  0009a3e6: ldr r0,[r0,#0x20]
  0009a3e8: cmp r0,#0x0
  0009a3ea: blt 0x0009a3f0
  0009a3ec: blx 0x00070a44
  0009a3f0: ldr r0,[r4,#0x24]
  0009a3f2: cmp r0,#0x0
  0009a3f4: blt 0x0009a3fa
  0009a3f6: blx 0x00070a44
  0009a3fa: ldr r0,[r4,#0x28]
  0009a3fc: cmp r0,#0x0
  0009a3fe: blt 0x0009a408
  0009a400: pop.w {r4,r6,r7,lr}
  0009a404: b.w 0x001ab778
  0009a408: pop {r4,r6,r7,pc}
```
