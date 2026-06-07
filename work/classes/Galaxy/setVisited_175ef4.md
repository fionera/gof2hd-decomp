# Galaxy::setVisited
elf 0x175ef4 body 46
Sig: undefined __thiscall setVisited(Galaxy * this, bool * param_1, int param_2)

## decompile
```c

/* Galaxy::setVisited(bool*, int) */

void __thiscall Galaxy::setVisited(Galaxy *this,bool *param_1,int param_2)

{
  int iVar1;
  
  for (iVar1 = 0; iVar1 < param_2; iVar1 = iVar1 + 1) {
    this->stations[iVar1] = param_1[iVar1];
  }
  for (; param_2 < 0x87; param_2 = param_2 + 1) {
    this->stations[param_2] = '\0';
  }
  return;
}

```

## target disasm
```
  00185ef4: push {r7,lr}
  00185ef6: mov r7,sp
  00185ef8: mov.w lr,#0x0
  00185efc: b 0x00185f0e
  00185efe: ldr.w r12,[r0,#0x0]
  00185f02: ldrb.w r3,[r1,lr]
  00185f06: strb.w r3,[r12,lr]
  00185f0a: add.w lr,lr,#0x1
  00185f0e: cmp lr,r2
  00185f10: blt 0x00185efe
  00185f12: movs r1,#0x0
  00185f14: cmp r2,#0x87
  00185f16: it ge
  00185f18: pop.ge {r7,pc}
  00185f1a: ldr r3,[r0,#0x0]
  00185f1c: strb r1,[r3,r2]
  00185f1e: adds r2,#0x1
  00185f20: b 0x00185f14
```
