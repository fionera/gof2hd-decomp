# FModSound::getPlayingProgress
elf 0x93988 body 130
Sig: undefined __thiscall getPlayingProgress(FModSound * this, int param_1)

## decompile
```c

/* FModSound::getPlayingProgress(int) */

void __thiscall FModSound::getPlayingProgress(FModSound *this,int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  uint in_fpscr;
  char *pcStack_1c;
  FMOD_EVENT_INFO aFStack_18 [4];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a3a10 + 0xa3998);
  local_14 = *piVar4;
  if ((((*(int *)(this + 0x2404) != 0) && (*(int *)(this + 0x23fc) != 0)) &&
      (this[0x11] != (FModSound)0x0)) && (*(int **)(this + param_1 * 4 + 0x18) != (int *)0x0)) {
    uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    FMOD::Event::getInfo(*(int **)(this + param_1 * 4 + 0x18),&pcStack_1c,aFStack_18);
    VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  }
  iVar3 = *piVar4 - local_14;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  000a3988: push {r4,r5,r7,lr}
  000a398a: add r7,sp,#0x8
  000a398c: sub sp,#0x40
  000a398e: ldr r2,[0x000a3a10]
  000a3990: vldr.32 s0,[pc,#0x78]
  000a3994: add r2,pc
  000a3996: ldr r4,[r2,#0x0]
  000a3998: ldr r2,[r4,#0x0]
  000a399a: str r2,[sp,#0x3c]
  000a399c: movw r2,#0x2404
  000a39a0: ldr r2,[r0,r2]
  000a39a2: cbz r2,0x000a39f6
  000a39a4: movw r2,#0x23fc
  000a39a8: ldr r2,[r0,r2]
  000a39aa: cbz r2,0x000a39f6
  000a39ac: ldrb r2,[r0,#0x11]
  000a39ae: cbz r2,0x000a39f6
  000a39b0: add.w r0,r0,r1, lsl #0x2
  000a39b4: ldr r0,[r0,#0x18]
  000a39b6: cbz r0,0x000a39f6
  000a39b8: vmov.i32 q8,#0x0
  000a39bc: mov r3,sp
  000a39be: add.w r1,r3,#0x20
  000a39c2: mov r5,r3
  000a39c4: add r2,sp,#0x38
  000a39c6: vst1.64 {d16,d17},[r1]
  000a39ca: add.w r1,r3,#0x10
  000a39ce: vst1.64 {d16,d17},[r1]
  000a39d2: movs r1,#0x0
  000a39d4: str r1,[sp,#0x30]
  000a39d6: movs r1,#0x8
  000a39d8: vst1.64 {d16,d17},[r5],r1
  000a39dc: add r1,sp,#0x34
  000a39de: blx 0x000715fc
  000a39e2: vldr.32 s0,[r5]
  000a39e6: vldr.32 s2,[sp,#0x4]
  000a39ea: vcvt.f32.s32 s0,s0
  000a39ee: vcvt.f32.s32 s2,s2
  000a39f2: vdiv.f32 s0,s2,s0
  000a39f6: ldr r0,[sp,#0x3c]
  000a39f8: ldr r1,[r4,#0x0]
  000a39fa: subs r0,r1,r0
  000a39fc: ittt eq
  000a39fe: vmov.eq r0,s0
  000a3a02: add.eq sp,#0x40
  000a3a04: pop.eq {r4,r5,r7,pc}
  000a3a06: blx 0x0006e824
```
