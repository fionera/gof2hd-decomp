# NFC::iap_buy_dlc_vip
elf 0x696e4 body 76
Sig: undefined __stdcall iap_buy_dlc_vip(void)

## decompile
```c

/* NFC::iap_buy_dlc_vip() */

void NFC::iap_buy_dlc_vip(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_00079730 + 0x796ee);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_00079738 + 0x796fe);
  **(undefined4 **)(DAT_00079734 + 0x796fc) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_0007973c + 0x7971c,DAT_00079740 + 0x7971e);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  000796e4: push {r4,r5,r7,lr}
  000796e6: add r7,sp,#0x8
  000796e8: ldr r0,[0x00079730]
  000796ea: add r0,pc
  000796ec: ldr r5,[r0,#0x0]
  000796ee: ldr r0,[r5,#0x0]
  000796f0: cbz r0,0x0007972e
  000796f2: ldr r1,[0x00079734]
  000796f4: movs r3,#0x1
  000796f6: ldr r2,[0x00079738]
  000796f8: add r1,pc
  000796fa: add r2,pc
  000796fc: ldr r1,[r1,#0x0]
  000796fe: ldr r2,[r2,#0x0]
  00079700: str r3,[r1,#0x0]
  00079702: ldr r3,[r0,#0x0]
  00079704: ldr r1,[r2,#0x0]
  00079706: ldr r2,[r3,#0x18]
  00079708: blx r2
  0007970a: mov r4,r0
  0007970c: ldr r0,[r5,#0x0]
  0007970e: ldr r1,[r0,#0x0]
  00079710: ldr r2,[0x0007973c]
  00079712: ldr r3,[0x00079740]
  00079714: ldr.w r12,[r1,#0x1c4]
  00079718: add r2,pc
  0007971a: add r3,pc
  0007971c: mov r1,r4
  0007971e: blx r12
  00079720: mov r2,r0
  00079722: ldr r0,[r5,#0x0]
  00079724: mov r1,r4
  00079726: pop.w {r4,r5,r7,lr}
  0007972a: b.w 0x001aae98
  0007972e: pop {r4,r5,r7,pc}
```
