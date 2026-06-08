# String::ReplaceChar
elf 0x73070 body 50
Sig: undefined __thiscall ReplaceChar(String * this, char param_1, char param_2)

## decompile
```c

/* AbyssEngine::String::ReplaceChar(char, char) */

void __thiscall AbyssEngine::String::ReplaceChar(String *this,char param_1,char param_2)

{
  int iVar1;
  ushort *puVar2;
  
  puVar2 = *(ushort **)(this + 4);
  if (puVar2 == (ushort *)0x0) {
    return;
  }
  iVar1 = *(int *)(this + 8);
  while (iVar1 != 0) {
    iVar1 = iVar1 + -1;
    if ((uint)*puVar2 == (int)param_1) {
      *puVar2 = (short)param_2;
    }
    puVar2 = puVar2 + 1;
  }
  return;
}

```

## target disasm
```
  00083070: push {r7,lr}
  00083072: mov r7,sp
  00083074: ldr.w lr,[r0,#0x4]
  00083078: cmp.w lr,#0x0
  0008307c: it eq
  0008307e: pop.eq {r7,pc}
  00083080: ldr.w r12,[r0,#0x8]
  00083084: b 0x0008309a
  00083086: ldrh.w r3,[lr,#0x0]
  0008308a: sub.w r12,r12,#0x1
  0008308e: cmp r3,r1
  00083090: it eq
  00083092: strh.eq.w r2,[lr,#0x0]
  00083096: add.w lr,lr,#0x2
  0008309a: cmp.w r12,#0x0
  0008309e: bne 0x00083086
  000830a0: pop {r7,pc}
```
