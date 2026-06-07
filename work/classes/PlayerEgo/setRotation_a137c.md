# PlayerEgo::setRotation
elf 0xa137c body 90
Sig: undefined __thiscall setRotation(PlayerEgo * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerEgo::setRotation(float, float, float) */

void __thiscall PlayerEgo::setRotation(PlayerEgo *this,float param_1,float param_2,float param_3)

{
  int *in_r1;
  int in_r2;
  int in_r3;
  uint *puVar1;
  int *piVar2;
  float extraout_s1;
  float extraout_s2;
  Matrix aMStack_58 [60];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000b13d8 + 0xb1390);
  puVar1 = *(uint **)(DAT_000b13dc + 0xb1392);
  local_1c = *piVar2;
  this[0x14].field_18 = in_r1;
  this[0x14].field_1C = in_r2;
  this[0x14].field_20 = in_r3;
  AbyssEngine::PaintCanvas::TransformGetLocal(*puVar1);
  AbyssEngine::AEMath::MatrixSetRotation
            (aMStack_58,(float)this[0x14].field_20,extraout_s1,extraout_s2);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b137c: push {r4,r5,r6,r7,lr}
  000b137e: add r7,sp,#0xc
  000b1380: push.w r8
  000b1384: sub sp,#0x48
  000b1386: mov r4,r0
  000b1388: ldr r0,[0x000b13d8]
  000b138a: ldr r5,[0x000b13dc]
  000b138c: add r0,pc
  000b138e: add r5,pc
  000b1390: ldr.w r8,[r0,#0x0]
  000b1394: ldr r5,[r5,#0x0]
  000b1396: ldr.w r0,[r8,#0x0]
  000b139a: str r0,[sp,#0x44]
  000b139c: add.w r0,r4,#0x2e8
  000b13a0: ldr r6,[r4,#0x4]
  000b13a2: stmia r0!,{r1,r2,r3}
  000b13a4: ldr r0,[r5,#0x0]
  000b13a6: ldr r1,[r6,#0xc]
  000b13a8: blx 0x000720c4
  000b13ac: ldrd r2,r3,[r4,#0x2e8]
  000b13b0: mov r1,r0
  000b13b2: vldr.32 s0,[r4,#0x2f0]
  000b13b6: add r0,sp,#0x8
  000b13b8: vstr.32 s0,[sp]
  000b13bc: blx 0x00072094
  000b13c0: ldr r0,[sp,#0x44]
  000b13c2: ldr.w r1,[r8,#0x0]
  000b13c6: subs r0,r1,r0
  000b13c8: ittt eq
  000b13ca: add.eq sp,#0x48
  000b13cc: pop.eq.w r8
  000b13d0: pop.eq {r4,r5,r6,r7,pc}
  000b13d2: blx 0x0006e824
```
