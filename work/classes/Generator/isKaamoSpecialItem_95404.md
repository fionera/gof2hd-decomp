# Generator::isKaamoSpecialItem
elf 0x95404 body 30
Sig: undefined __thiscall isKaamoSpecialItem(Generator * this, int param_1)

## decompile
```c

/* Generator::isKaamoSpecialItem(int) */

void __thiscall Generator::isKaamoSpecialItem(Generator *this,int param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    if (8 < uVar2) {
      return;
    }
    iVar1 = uVar2 * 4;
    uVar2 = uVar2 + 1;
  } while (*(int *)(DAT_000a5424 + 0xa540c + iVar1) != param_1);
  return;
}

```

## target disasm
```
  000a5404: ldr r2,[0x000a5424]
  000a5406: movs r0,#0x0
  000a5408: add r2,pc
  000a540a: cmp r0,#0x8
  000a540c: itt hi
  000a540e: mov.hi r0,#0x0
  000a5410: bx.hi lr
  000a5412: ldr.w r3,[r2,r0,lsl #0x2]
  000a5416: adds r0,#0x1
  000a5418: cmp r3,r1
  000a541a: itt eq
  000a541c: mov.eq r0,#0x1
  000a541e: bx.eq lr
  000a5420: b 0x000a540a
```
