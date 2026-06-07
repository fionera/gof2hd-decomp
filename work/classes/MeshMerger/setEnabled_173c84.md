# MeshMerger::setEnabled
elf 0x173c84 body 34
Sig: undefined __thiscall setEnabled(MeshMerger * this, int param_1, bool param_2)

## decompile
```c

/* MeshMerger::setEnabled(int, bool) */

void __thiscall MeshMerger::setEnabled(MeshMerger *this,int param_1,bool param_2)

{
  if ((bool)*(char *)(*(int *)(this + 0x28) + param_1) != param_2) {
    *(bool *)(*(int *)(this + 0x28) + param_1) = param_2;
    if (*(char *)(*(int *)(this + 0x2c) + param_1) == '\0') {
      return;
    }
    this[0x34] = (MeshMerger)0x1;
  }
  return;
}

```

## target disasm
```
  00183c84: ldr.w r12,[r0,#0x28]
  00183c88: ldrb.w r3,[r12,r1]
  00183c8c: cmp r3,r2
  00183c8e: beq 0x00183ca4
  00183c90: strb.w r2,[r12,r1]
  00183c94: ldr r2,[r0,#0x2c]
  00183c96: ldrb r1,[r2,r1]
  00183c98: cmp r1,#0x0
  00183c9a: it eq
  00183c9c: bx.eq lr
  00183c9e: movs r1,#0x1
  00183ca0: strb.w r1,[r0,#0x34]
  00183ca4: bx lr
```
