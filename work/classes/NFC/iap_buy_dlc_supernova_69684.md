# NFC::iap_buy_dlc_supernova
elf 0x69684 body 76
Sig: undefined __stdcall iap_buy_dlc_supernova(void)

## decompile
```c

/* NFC::iap_buy_dlc_supernova() */

void NFC::iap_buy_dlc_supernova(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_000796d0 + 0x7968e);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_000796d8 + 0x7969e);
  **(undefined4 **)(DAT_000796d4 + 0x7969c) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_000796dc + 0x796bc,DAT_000796e0 + 0x796be);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079684: push {r4,r5,r7,lr}
  00079686: add r7,sp,#0x8
  00079688: ldr r0,[0x000796d0]
  0007968a: add r0,pc
  0007968c: ldr r5,[r0,#0x0]
  0007968e: ldr r0,[r5,#0x0]
  00079690: cbz r0,0x000796ce
  00079692: ldr r1,[0x000796d4]
  00079694: movs r3,#0x1
  00079696: ldr r2,[0x000796d8]
  00079698: add r1,pc
  0007969a: add r2,pc
  0007969c: ldr r1,[r1,#0x0]
  0007969e: ldr r2,[r2,#0x0]
  000796a0: str r3,[r1,#0x0]
  000796a2: ldr r3,[r0,#0x0]
  000796a4: ldr r1,[r2,#0x0]
  000796a6: ldr r2,[r3,#0x18]
  000796a8: blx r2
  000796aa: mov r4,r0
  000796ac: ldr r0,[r5,#0x0]
  000796ae: ldr r1,[r0,#0x0]
  000796b0: ldr r2,[0x000796dc]
  000796b2: ldr r3,[0x000796e0]
  000796b4: ldr.w r12,[r1,#0x1c4]
  000796b8: add r2,pc
  000796ba: add r3,pc
  000796bc: mov r1,r4
  000796be: blx r12
  000796c0: mov r2,r0
  000796c2: ldr r0,[r5,#0x0]
  000796c4: mov r1,r4
  000796c6: pop.w {r4,r5,r7,lr}
  000796ca: b.w 0x001aae98
  000796ce: pop {r4,r5,r7,pc}
```
