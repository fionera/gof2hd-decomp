# LodMeshMerger::setEnabled
elf 0x181438 body 34
Sig: undefined __thiscall setEnabled(LodMeshMerger * this, int param_1, bool param_2)

## decompile
```c

/* LodMeshMerger::setEnabled(int, bool) */

void __thiscall LodMeshMerger::setEnabled(LodMeshMerger *this,int param_1,bool param_2)

{
  if ((bool)*(char *)(*(int *)(this + 0x30) + param_1) != param_2) {
    *(bool *)(*(int *)(this + 0x30) + param_1) = param_2;
    if (*(char *)(*(int *)(this + 0x34) + param_1) == '\0') {
      return;
    }
    this[0x3c] = (LodMeshMerger)0x1;
  }
  return;
}

```

## target disasm
```
  00191438: ldr.w r12,[r0,#0x30]
  0019143c: ldrb.w r3,[r12,r1]
  00191440: cmp r3,r2
  00191442: beq 0x00191458
  00191444: strb.w r2,[r12,r1]
  00191448: ldr r2,[r0,#0x34]
  0019144a: ldrb r1,[r2,r1]
  0019144c: cmp r1,#0x0
  0019144e: it eq
  00191450: bx.eq lr
  00191452: movs r1,#0x1
  00191454: strb.w r1,[r0,#0x3c]
  00191458: bx lr
```
