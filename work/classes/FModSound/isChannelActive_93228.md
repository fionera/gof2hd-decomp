# FModSound::isChannelActive
elf 0x93228 body 62
Sig: undefined __stdcall isChannelActive(int param_1)

## decompile
```c

/* FModSound::isChannelActive(int) */

void FModSound::isChannelActive(int param_1)

{
  uint *puVar1;
  uint uVar2;
  int in_r1;
  uint in_r2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000a3268 + 0xa3232);
  iVar3 = *piVar4;
  if ((*(int *)(param_1 + 0x23fc) == 0) ||
     (puVar1 = *(uint **)(param_1 + in_r1 * 4 + 0x18), puVar1 == (uint *)0x0)) {
    uVar2 = 0;
  }
  else {
    FMOD::Event::getState(puVar1);
    uVar2 = (in_r2 & 0x1f) >> 4;
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
  000a3228: push {r2,r3,r4,r6,r7,lr}
  000a322a: add r7,sp,#0x10
  000a322c: ldr r2,[0x000a3268]
  000a322e: add r2,pc
  000a3230: ldr r4,[r2,#0x0]
  000a3232: ldr r2,[r4,#0x0]
  000a3234: str r2,[sp,#0x4]
  000a3236: movw r2,#0x23fc
  000a323a: ldr r2,[r0,r2]
  000a323c: cbz r2,0x000a3254
  000a323e: add.w r0,r0,r1, lsl #0x2
  000a3242: ldr r0,[r0,#0x18]
  000a3244: cbz r0,0x000a3254
  000a3246: mov r1,sp
  000a3248: blx 0x000714e8
  000a324c: ldr r0,[sp,#0x0]
  000a324e: ubfx r0,r0,#0x4,#0x1
  000a3252: b 0x000a3256
  000a3254: movs r0,#0x0
  000a3256: ldr r1,[sp,#0x4]
  000a3258: ldr r2,[r4,#0x0]
  000a325a: subs r1,r2,r1
  000a325c: itt eq
  000a325e: add.eq sp,#0x8
  000a3260: pop.eq {r4,r6,r7,pc}
  000a3262: blx 0x0006e824
```
