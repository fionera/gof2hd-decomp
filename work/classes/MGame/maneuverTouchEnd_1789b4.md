# MGame::maneuverTouchEnd
elf 0x1789b4 body 104
Sig: undefined __stdcall maneuverTouchEnd(int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::maneuverTouchEnd(int, int, void*) */

void MGame::maneuverTouchEnd(int param_1,int param_2,void *param_3)

{
  int iVar1;
  uint in_fpscr;
  float fVar2;
  float fVar3;
  
  if ((*(char *)(param_1 + 0x17c) != '\0') && (*(int *)(param_1 + 0x178) < 0x259)) {
    fVar2 = (float)VectorSignedToFloat(**(undefined4 **)(DAT_00188a24 + 0x1889d4),
                                       (byte)(in_fpscr >> 0x16) & 3);
    iVar1 = param_2 - *(int *)(param_1 + 0x180);
    if (iVar1 < 0) {
      iVar1 = -iVar1;
    }
    fVar3 = (float)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    if ((int)((uint)((fVar2 / DAT_00188a1c) * DAT_00188a20 < fVar3) << 0x1f) < 0) {
      iVar1 = 1;
      if (*(int *)(param_1 + 0x180) < param_2) {
        iVar1 = 2;
      }
      PlayerEgo::initManeuver(*(PlayerEgo **)(param_1 + 0x58),iVar1);
    }
  }
  *(undefined1 *)(param_1 + 0x17c) = 0;
  return;
}

```

## target disasm
```
  001889b4: push {r4,r6,r7,lr}
  001889b6: add r7,sp,#0x8
  001889b8: mov r4,r0
  001889ba: ldrb.w r0,[r0,#0x17c]
  001889be: cbz r0,0x00188a14
  001889c0: ldr.w r0,[r4,#0x178]
  001889c4: cmp.w r0,#0x258
  001889c8: bgt 0x00188a14
  001889ca: ldr r0,[0x00188a24]
  001889cc: vldr.32 s2,[pc,#0x4c]
  001889d0: add r0,pc
  001889d2: ldr r0,[r0,#0x0]
  001889d4: vldr.32 s0,[r0]
  001889d8: vcvt.f32.s32 s0,s0
  001889dc: ldr.w r3,[r4,#0x180]
  001889e0: subs r0,r1,r3
  001889e2: it mi
  001889e4: rsb.mi r0,r0
  001889e6: vmov s4,r0
  001889ea: vcvt.f32.s32 s4,s4
  001889ee: vdiv.f32 s0,s0,s2
  001889f2: vldr.32 s2,[pc,#0x2c]
  001889f6: vmul.f32 s0,s0,s2
  001889fa: vcmpe.f32 s0,s4
  001889fe: vmrs apsr,fpscr
  00188a02: bpl 0x00188a14
  00188a04: movs r2,#0x1
  00188a06: ldr r0,[r4,#0x58]
  00188a08: cmp r3,r1
  00188a0a: it lt
  00188a0c: mov.lt r2,#0x2
  00188a0e: mov r1,r2
  00188a10: blx 0x00078154
  00188a14: movs r0,#0x0
  00188a16: strb.w r0,[r4,#0x17c]
  00188a1a: pop {r4,r6,r7,pc}
```
