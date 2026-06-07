# GameText::isNonArabicString
elf 0x7f798 body 86
Sig: undefined __stdcall isNonArabicString(ushort * param_1, uint param_2)

## decompile
```c

/* GameText::isNonArabicString(unsigned short const*, unsigned int) */

undefined4 GameText::isNonArabicString(ushort *param_1,uint param_2)

{
  bool bVar1;
  int iVar2;
  ushort uVar3;
  int iVar4;
  int iVar5;
  
  uVar3 = 0;
  do {
    if (param_2 <= uVar3) {
      return 1;
    }
    bVar1 = true;
    iVar5 = DAT_0008f7f0 + 0x8f7ac;
    for (iVar4 = 0; iVar4 != 0x29; iVar4 = iVar4 + 1) {
      for (iVar2 = 0; iVar2 != 5; iVar2 = iVar2 + 1) {
        bVar1 = (bool)(bVar1 & *(uint *)(iVar5 + iVar2 * 4) != (uint)param_1[uVar3]);
      }
      iVar5 = iVar5 + 0x14;
    }
    uVar3 = uVar3 + 1;
  } while (bVar1);
  return 0;
}

```

## target disasm
```
  0008f798: push {r4,r5,r6,r7,lr}
  0008f79a: add r7,sp,#0xc
  0008f79c: push.w r8
  0008f7a0: mov r12,r0
  0008f7a2: ldr r0,[0x0008f7f0]
  0008f7a4: mov lr,r1
  0008f7a6: movs r2,#0x0
  0008f7a8: add r0,pc
  0008f7aa: mov r8,r0
  0008f7ac: uxth r0,r2
  0008f7ae: cmp r0,lr
  0008f7b0: bcs 0x0008f7e6
  0008f7b2: ldrh.w r5,[r12,r0,lsl #0x1]
  0008f7b6: movs r3,#0x0
  0008f7b8: movs r4,#0x1
  0008f7ba: mov r6,r8
  0008f7bc: b 0x0008f7d8
  0008f7be: movs r0,#0x0
  0008f7c0: b 0x0008f7d0
  0008f7c2: ldr.w r1,[r6,r0,lsl #0x2]
  0008f7c6: adds r0,#0x1
  0008f7c8: subs r1,r1,r5
  0008f7ca: it ne
  0008f7cc: mov.ne r1,#0x1
  0008f7ce: ands r4,r1
  0008f7d0: cmp r0,#0x5
  0008f7d2: bne 0x0008f7c2
  0008f7d4: adds r6,#0x14
  0008f7d6: adds r3,#0x1
  0008f7d8: cmp r3,#0x29
  0008f7da: bne 0x0008f7be
  0008f7dc: adds r2,#0x1
  0008f7de: lsls r0,r4,#0x1f
  0008f7e0: bne 0x0008f7ac
  0008f7e2: movs r0,#0x0
  0008f7e4: b 0x0008f7e8
  0008f7e6: movs r0,#0x1
  0008f7e8: pop.w r8
  0008f7ec: pop {r4,r5,r6,r7,pc}
```
