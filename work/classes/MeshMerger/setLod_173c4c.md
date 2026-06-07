# MeshMerger::setLod
elf 0x173c4c body 40
Sig: undefined __thiscall setLod(MeshMerger * this, int param_1, signed param_2)

## decompile
```c

/* MeshMerger::setLod(int, signed char) */

void __thiscall MeshMerger::setLod(MeshMerger *this,int param_1,char param_3)

{
  if (*(char *)(*(int *)(this + 0x24) + param_1) != param_3) {
    *(char *)(*(int *)(this + 0x24) + param_1) = param_3;
    if (*(char *)(*(int *)(this + 0x28) + param_1) == '\0') {
      return;
    }
    this[0x34] = (MeshMerger)0x1;
  }
  return;
}

```

## target disasm
```
  00183c4c: push {r7,lr}
  00183c4e: mov r7,sp
  00183c50: ldr.w lr,[r0,#0x24]
  00183c54: uxtb r3,r2
  00183c56: ldrb.w r12,[lr,r1]
  00183c5a: cmp r12,r3
  00183c5c: beq 0x00183c72
  00183c5e: strb.w r2,[lr,r1]
  00183c62: ldr r2,[r0,#0x28]
  00183c64: ldrb r1,[r2,r1]
  00183c66: cmp r1,#0x0
  00183c68: it eq
  00183c6a: pop.eq {r7,pc}
  00183c6c: movs r1,#0x1
  00183c6e: strb.w r1,[r0,#0x34]
  00183c72: pop {r7,pc}
```
