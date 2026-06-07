# FModSound::isPlaying
elf 0x931e4 body 62
Sig: undefined __stdcall isPlaying(int param_1)

## decompile
```c

/* FModSound::isPlaying(int) */

void FModSound::isPlaying(int param_1)

{
  uint *puVar1;
  uint uVar2;
  int in_r1;
  uint in_r2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000a3224 + 0xa31ee);
  iVar3 = *piVar4;
  if ((*(int *)(param_1 + 0x23fc) == 0) ||
     (puVar1 = *(uint **)(param_1 + in_r1 * 4 + 0x18), puVar1 == (uint *)0x0)) {
    uVar2 = 0;
  }
  else {
    FMOD::Event::getState(puVar1);
    uVar2 = (in_r2 & 0xf) >> 3;
  }
  if (*piVar4 != iVar3) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar2);
  }
  return;
}

```

## target disasm
```
  000a31e4: push {r2,r3,r4,r6,r7,lr}
  000a31e6: add r7,sp,#0x10
  000a31e8: ldr r2,[0x000a3224]
  000a31ea: add r2,pc
  000a31ec: ldr r4,[r2,#0x0]
  000a31ee: ldr r2,[r4,#0x0]
  000a31f0: str r2,[sp,#0x4]
  000a31f2: movw r2,#0x23fc
  000a31f6: ldr r2,[r0,r2]
  000a31f8: cbz r2,0x000a3210
  000a31fa: add.w r0,r0,r1, lsl #0x2
  000a31fe: ldr r0,[r0,#0x18]
  000a3200: cbz r0,0x000a3210
  000a3202: mov r1,sp
  000a3204: blx 0x000714e8
  000a3208: ldr r0,[sp,#0x0]
  000a320a: ubfx r0,r0,#0x3,#0x1
  000a320e: b 0x000a3212
  000a3210: movs r0,#0x0
  000a3212: ldr r1,[sp,#0x4]
  000a3214: ldr r2,[r4,#0x0]
  000a3216: subs r1,r2,r1
  000a3218: itt eq
  000a321a: add.eq sp,#0x8
  000a321c: pop.eq {r4,r6,r7,pc}
  000a321e: blx 0x0006e824
```
