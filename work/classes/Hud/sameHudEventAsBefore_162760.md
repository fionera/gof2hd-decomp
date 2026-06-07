# Hud::sameHudEventAsBefore
elf 0x162760 body 62
Sig: undefined __thiscall sameHudEventAsBefore(Hud * this, String param_1)

## decompile
```c

/* Hud::sameHudEventAsBefore(AbyssEngine::String) */

undefined4 __thiscall Hud::sameHudEventAsBefore(Hud *this,String *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = **(int **)(this + 0x264);
  do {
    iVar1 = iVar1 + -1;
    if (iVar1 < 1) {
      return 0;
    }
    iVar2 = *(int *)(*(int *)(*(int *)(this + 0x264) + 4) + iVar1 * 4);
  } while ((iVar2 == 0) ||
          (iVar2 = AbyssEngine::String::Compare(*(String **)(iVar2 + 0x1c),param_2), iVar2 != 0));
  return 1;
}

```

## target disasm
```
  00172760: push {r4,r5,r6,r7,lr}
  00172762: add r7,sp,#0xc
  00172764: push.w r11
  00172768: mov r5,r0
  0017276a: ldr.w r0,[r0,#0x264]
  0017276e: mov r4,r1
  00172770: ldr r0,[r0,#0x0]
  00172772: subs r6,r0,#0x1
  00172774: b 0x0017278e
  00172776: ldr.w r0,[r5,#0x264]
  0017277a: ldr r0,[r0,#0x4]
  0017277c: ldr.w r0,[r0,r6,lsl #0x2]
  00172780: cbz r0,0x0017278c
  00172782: ldr r0,[r0,#0x1c]
  00172784: mov r1,r4
  00172786: blx 0x0006f664
  0017278a: cbz r0,0x00172796
  0017278c: subs r6,#0x1
  0017278e: cmp r6,#0x1
  00172790: bge 0x00172776
  00172792: movs r0,#0x0
  00172794: b 0x00172798
  00172796: movs r0,#0x1
  00172798: pop.w r11
  0017279c: pop {r4,r5,r6,r7,pc}
```
