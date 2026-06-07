# HangarList::getCurrentLength
elf 0x11e786 body 20
Sig: undefined __thiscall getCurrentLength(HangarList * this)

## decompile
```c

/* HangarList::getCurrentLength() */

undefined4 __thiscall HangarList::getCurrentLength(HangarList *this)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = *(undefined4 **)(*(int *)(*(int *)this + 4) + *(int *)(this + 4) * 4);
  if (puVar1 == (undefined4 *)0x0) {
    uVar2 = 0;
  }
  else {
    uVar2 = *puVar1;
  }
  return uVar2;
}

```

## target disasm
```
  0012e786: ldrd r1,r0,[r0,#0x0]
  0012e78a: ldr r1,[r1,#0x4]
  0012e78c: ldr.w r0,[r1,r0,lsl #0x2]
  0012e790: cmp r0,#0x0
  0012e792: ite ne
  0012e794: ldr.ne r0,[r0,#0x0]
  0012e796: mov.eq r0,#0x0
  0012e798: bx lr
```
