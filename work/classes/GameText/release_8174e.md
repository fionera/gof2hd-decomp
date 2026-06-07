# GameText::release
elf 0x8174e body 66
Sig: undefined __thiscall release(GameText * this)

## decompile
```c

/* GameText::release() */

void __thiscall GameText::release(GameText *this)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  
  if (*(int *)(this + 0xc) != 0) {
    iVar4 = 0;
    for (iVar3 = 0; iVar3 < *(int *)(this + 0x1c); iVar3 = iVar3 + 1) {
      piVar1 = *(int **)(*(int *)(this + 0xc) + iVar4);
      if (piVar1 == (int *)0x0) {
        puVar2 = (undefined4 *)(*(int *)(this + 0xc) + iVar3 * 4);
      }
      else {
        (**(code **)(*piVar1 + 4))();
        puVar2 = (undefined4 *)(*(int *)(this + 0xc) + iVar4);
      }
      *puVar2 = 0;
      iVar4 = iVar4 + 4;
    }
  }
  return;
}

```

## target disasm
```
  0009174e: push {r4,r5,r6,r7,lr}
  00091750: add r7,sp,#0xc
  00091752: push.w r8
  00091756: mov r4,r0
  00091758: ldr r0,[r0,#0xc]
  0009175a: cbz r0,0x0009178a
  0009175c: mov.w r8,#0x0
  00091760: movs r6,#0x0
  00091762: movs r5,#0x0
  00091764: b 0x00091784
  00091766: ldr r1,[r4,#0xc]
  00091768: ldr r0,[r1,r6]
  0009176a: cbz r0,0x00091778
  0009176c: ldr r1,[r0,#0x0]
  0009176e: ldr r1,[r1,#0x4]
  00091770: blx r1
  00091772: ldr r0,[r4,#0xc]
  00091774: add r0,r6
  00091776: b 0x0009177c
  00091778: add.w r0,r1,r5, lsl #0x2
  0009177c: str.w r8,[r0,#0x0]
  00091780: adds r6,#0x4
  00091782: adds r5,#0x1
  00091784: ldr r0,[r4,#0x1c]
  00091786: cmp r5,r0
  00091788: blt 0x00091766
  0009178a: pop.w r8
  0009178e: pop {r4,r5,r6,r7,pc}
```
