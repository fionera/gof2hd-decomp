# NFC::showMoreGames
elf 0x69b28 body 66
Sig: undefined __stdcall showMoreGames(void)

## decompile
```c

/* NFC::showMoreGames() */

void NFC::showMoreGames(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079b6c + 0x79b32);
  piVar1 = (int *)*piVar4;
  if (piVar1 == (int *)0x0) {
    return;
  }
  uVar2 = (**(code **)(*piVar1 + 0x18))(piVar1,**(undefined4 **)(DAT_00079b70 + 0x79b3e));
  uVar3 = (**(code **)(*(int *)*piVar4 + 0x1c4))
                    ((int *)*piVar4,uVar2,DAT_00079b74 + 0x79b56,DAT_00079b78 + 0x79b58);
                    /* WARNING: Could not recover jumptable at 0x001aaea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001aaea4 + 0x1aaea8))(*piVar4,uVar2,uVar3);
  return;
}

```

## target disasm
```
  00079b28: push {r4,r5,r7,lr}
  00079b2a: add r7,sp,#0x8
  00079b2c: ldr r0,[0x00079b6c]
  00079b2e: add r0,pc
  00079b30: ldr r5,[r0,#0x0]
  00079b32: ldr r0,[r5,#0x0]
  00079b34: cbz r0,0x00079b68
  00079b36: ldr r1,[0x00079b70]
  00079b38: ldr r2,[r0,#0x0]
  00079b3a: add r1,pc
  00079b3c: ldr r1,[r1,#0x0]
  00079b3e: ldr r2,[r2,#0x18]
  00079b40: ldr r1,[r1,#0x0]
  00079b42: blx r2
  00079b44: mov r4,r0
  00079b46: ldr r0,[r5,#0x0]
  00079b48: ldr r1,[r0,#0x0]
  00079b4a: ldr r2,[0x00079b74]
  00079b4c: ldr r3,[0x00079b78]
  00079b4e: ldr.w r12,[r1,#0x1c4]
  00079b52: add r2,pc
  00079b54: add r3,pc
  00079b56: mov r1,r4
  00079b58: blx r12
  00079b5a: mov r2,r0
  00079b5c: ldr r0,[r5,#0x0]
  00079b5e: mov r1,r4
  00079b60: pop.w {r4,r5,r7,lr}
  00079b64: b.w 0x001aae98
  00079b68: pop {r4,r5,r7,pc}
```
