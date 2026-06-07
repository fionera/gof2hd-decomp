# NFC::iap_buy_dlc_full_package
elf 0x69744 body 76
Sig: undefined __stdcall iap_buy_dlc_full_package(void)

## decompile
```c

/* NFC::iap_buy_dlc_full_package() */

void NFC::iap_buy_dlc_full_package(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_00079790 + 0x7974e);
  piVar1 = (int *)*piVar5;
  if (piVar1 == (int *)0x0) {
    return;
  }
  puVar4 = *(undefined4 **)(DAT_00079798 + 0x7975e);
  **(undefined4 **)(DAT_00079794 + 0x7975c) = 1;
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,*puVar4);
  uVar3 = (**(code **)(*(int *)*piVar5 + 0x1c4))
                    ((int *)*piVar5,uVar2,DAT_0007979c + 0x7977c,DAT_000797a0 + 0x7977e);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar5,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079744: push {r4,r5,r7,lr}
  00079746: add r7,sp,#0x8
  00079748: ldr r0,[0x00079790]
  0007974a: add r0,pc
  0007974c: ldr r5,[r0,#0x0]
  0007974e: ldr r0,[r5,#0x0]
  00079750: cbz r0,0x0007978e
  00079752: ldr r1,[0x00079794]
  00079754: movs r3,#0x1
  00079756: ldr r2,[0x00079798]
  00079758: add r1,pc
  0007975a: add r2,pc
  0007975c: ldr r1,[r1,#0x0]
  0007975e: ldr r2,[r2,#0x0]
  00079760: str r3,[r1,#0x0]
  00079762: ldr r3,[r0,#0x0]
  00079764: ldr r1,[r2,#0x0]
  00079766: ldr r2,[r3,#0x18]
  00079768: blx r2
  0007976a: mov r4,r0
  0007976c: ldr r0,[r5,#0x0]
  0007976e: ldr r1,[r0,#0x0]
  00079770: ldr r2,[0x0007979c]
  00079772: ldr r3,[0x000797a0]
  00079774: ldr.w r12,[r1,#0x1c4]
  00079778: add r2,pc
  0007977a: add r3,pc
  0007977c: mov r1,r4
  0007977e: blx r12
  00079780: mov r2,r0
  00079782: ldr r0,[r5,#0x0]
  00079784: mov r1,r4
  00079786: pop.w {r4,r5,r7,lr}
  0007978a: b.w 0x001aae98
  0007978e: pop {r4,r5,r7,pc}
```
