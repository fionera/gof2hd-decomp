# PlayerEgo::setTurretPosition
elf 0x9b2bc body 44
Sig: undefined __stdcall setTurretPosition(Vector param_1)

## decompile
```c

/* PlayerEgo::setTurretPosition(AbyssEngine::AEMath::Vector) */

undefined8
PlayerEgo::setTurretPosition(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_000ab2e8 + 0xab2ce);
  local_14 = *piVar1;
  local_20 = param_2;
  uStack_1c = param_3;
  local_18 = param_4;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x224),(Vector *)&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000ab2bc: push {r0,r1,r2,r3,r4,r6,r7,lr}
  000ab2be: add r7,sp,#0x18
  000ab2c0: ldr r4,[0x000ab2e8]
  000ab2c2: add.w r0,r0,#0x224
  000ab2c6: strd r1,r2,[sp,#0x0]
  000ab2ca: add r4,pc
  000ab2cc: ldr r4,[r4,#0x0]
  000ab2ce: ldr r1,[r4,#0x0]
  000ab2d0: str r3,[sp,#0x8]
  000ab2d2: str r1,[sp,#0xc]
  000ab2d4: mov r1,sp
  000ab2d6: blx 0x0006eb3c
  000ab2da: ldr r0,[sp,#0xc]
  000ab2dc: ldr r1,[r4,#0x0]
  000ab2de: subs r0,r1,r0
  000ab2e0: it eq
  000ab2e2: pop.eq {r0,r1,r2,r3,r4,r6,r7,pc}
  000ab2e4: blx 0x0006e824
```
