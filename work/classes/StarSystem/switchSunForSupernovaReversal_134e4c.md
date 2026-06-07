# StarSystem::switchSunForSupernovaReversal
elf 0x134e4c body 72
Sig: undefined __stdcall switchSunForSupernovaReversal(void)

## decompile
```c

/* StarSystem::switchSunForSupernovaReversal() */

undefined8 StarSystem::switchSunForSupernovaReversal(void)

{
  int in_r0;
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_00144e98 + 0x144e5c);
  local_14 = *piVar1;
  AbyssEngine::PaintCanvas::TextureCreate
            ((ushort)**(undefined4 **)(DAT_00144e9c + 0x144e5e),(uint *)0x2734,
             SUB41(*(undefined4 *)(*(int *)(in_r0 + 0x14) + 4),0));
  local_20 = DAT_00144e94;
  uStack_1c = DAT_00144e94;
  local_18 = DAT_00144e94;
  AEGeometry::setScaling
            ((AEGeometry *)**(undefined4 **)(*(int *)(in_r0 + 0x1c) + 4),(Vector *)&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00144e4c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  00144e4e: add r7,sp,#0x18
  00144e50: mov r4,r0
  00144e52: ldr r0,[0x00144e98]
  00144e54: ldr r1,[0x00144e9c]
  00144e56: movs r3,#0x0
  00144e58: add r0,pc
  00144e5a: add r1,pc
  00144e5c: ldr r5,[r0,#0x0]
  00144e5e: ldr r1,[r1,#0x0]
  00144e60: ldr r0,[r5,#0x0]
  00144e62: str r0,[sp,#0xc]
  00144e64: ldr r2,[r4,#0x14]
  00144e66: ldr r0,[r1,#0x0]
  00144e68: movw r1,#0x2734
  00144e6c: ldr r2,[r2,#0x4]
  00144e6e: blx 0x0006fbb0
  00144e72: ldr r0,[r4,#0x1c]
  00144e74: ldr r1,[0x00144e94]
  00144e76: ldr r0,[r0,#0x4]
  00144e78: ldr r0,[r0,#0x0]
  00144e7a: strd r1,r1,[sp,#0x0]
  00144e7e: str r1,[sp,#0x8]
  00144e80: mov r1,sp
  00144e82: blx 0x00073de0
  00144e86: ldr r0,[sp,#0xc]
  00144e88: ldr r1,[r5,#0x0]
  00144e8a: subs r0,r1,r0
  00144e8c: it eq
  00144e8e: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  00144e90: blx 0x0006e824
```
