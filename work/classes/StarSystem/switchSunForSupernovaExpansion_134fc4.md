# StarSystem::switchSunForSupernovaExpansion
elf 0x134fc4 body 48
Sig: undefined __thiscall switchSunForSupernovaExpansion(StarSystem * this)

## decompile
```c

/* StarSystem::switchSunForSupernovaExpansion() */

undefined8 __thiscall StarSystem::switchSunForSupernovaExpansion(StarSystem *this)

{
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_00144ff8 + 0x144fce);
  local_14 = *piVar1;
  local_20 = DAT_00144ff4;
  uStack_1c = DAT_00144ff4;
  local_18 = DAT_00144ff4;
  AEGeometry::setScaling
            ((AEGeometry *)**(undefined4 **)(*(int *)(this + 0x1c) + 4),(Vector *)&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00144fc4: push {r0,r1,r2,r3,r4,r6,r7,lr}
  00144fc6: add r7,sp,#0x18
  00144fc8: ldr r1,[0x00144ff8]
  00144fca: add r1,pc
  00144fcc: ldr r4,[r1,#0x0]
  00144fce: ldr r1,[r4,#0x0]
  00144fd0: str r1,[sp,#0xc]
  00144fd2: ldr r0,[r0,#0x1c]
  00144fd4: ldr r1,[0x00144ff4]
  00144fd6: ldr r0,[r0,#0x4]
  00144fd8: ldr r0,[r0,#0x0]
  00144fda: strd r1,r1,[sp,#0x0]
  00144fde: str r1,[sp,#0x8]
  00144fe0: mov r1,sp
  00144fe2: blx 0x00073de0
  00144fe6: ldr r0,[sp,#0xc]
  00144fe8: ldr r1,[r4,#0x0]
  00144fea: subs r0,r1,r0
  00144fec: it eq
  00144fee: pop.eq {r0,r1,r2,r3,r4,r6,r7,pc}
  00144ff0: blx 0x0006e824
```
