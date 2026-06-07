# Trail::reset
elf 0x15c290 body 116
Sig: undefined __stdcall reset(Vector param_1)

## decompile
```c

/* Trail::reset(AbyssEngine::AEMath::Vector) */

void Trail::reset(Vector *param_1,float param_2,float param_3,float param_4)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  float local_28;
  float local_24;
  float local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0016c304 + 0x16c2ba);
  local_1c = *piVar3;
  for (iVar1 = 0; iVar1 < *(int *)(param_1 + 0x20); iVar1 = iVar1 + 3) {
    iVar2 = *(int *)(param_1 + 0x18);
    *(int *)(iVar2 + iVar1 * 4) = (int)param_2;
    iVar2 = iVar2 + iVar1 * 4;
    *(int *)(iVar2 + 4) = (int)param_3;
    *(int *)(iVar2 + 8) = (int)param_4;
  }
  local_28 = param_2;
  local_24 = param_3;
  local_20 = param_4;
  update(param_1,(Vector *)&local_28);
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016c290: push {r4,r5,r6,r7,lr}
  0016c292: add r7,sp,#0xc
  0016c294: push.w r11
  0016c298: sub sp,#0x10
  0016c29a: vmov s0,r1
  0016c29e: ldr r4,[0x0016c304]
  0016c2a0: vmov s2,r3
  0016c2a4: str r1,[sp,#0x0]
  0016c2a6: vmov s4,r2
  0016c2aa: str r2,[sp,#0x4]
  0016c2ac: vcvt.s32.f32 s2,s2
  0016c2b0: str r3,[sp,#0x8]
  0016c2b2: vcvt.s32.f32 s4,s4
  0016c2b6: add r4,pc
  0016c2b8: vcvt.s32.f32 s0,s0
  0016c2bc: ldr r6,[r4,#0x0]
  0016c2be: movs r4,#0x0
  0016c2c0: ldr r1,[r6,#0x0]
  0016c2c2: str r1,[sp,#0xc]
  0016c2c4: vmov r1,s2
  0016c2c8: vmov r2,s4
  0016c2cc: vmov r3,s0
  0016c2d0: b 0x0016c2e2
  0016c2d2: ldr r5,[r0,#0x18]
  0016c2d4: str.w r3,[r5,r4,lsl #0x2]
  0016c2d8: add.w r5,r5,r4, lsl #0x2
  0016c2dc: adds r4,#0x3
  0016c2de: strd r2,r1,[r5,#0x4]
  0016c2e2: ldr r5,[r0,#0x20]
  0016c2e4: cmp r4,r5
  0016c2e6: blt 0x0016c2d2
  0016c2e8: mov r1,sp
  0016c2ea: mov r2,r1
  0016c2ec: blx 0x00075820
  0016c2f0: ldr r0,[sp,#0xc]
  0016c2f2: ldr r1,[r6,#0x0]
  0016c2f4: subs r0,r1,r0
  0016c2f6: ittt eq
  0016c2f8: add.eq sp,#0x10
  0016c2fa: pop.eq.w r11
  0016c2fe: pop.eq {r4,r5,r6,r7,pc}
  0016c300: blx 0x0006e824
```
