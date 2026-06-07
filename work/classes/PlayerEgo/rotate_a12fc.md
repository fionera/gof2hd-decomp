# PlayerEgo::rotate
elf 0xa12fc body 118
Sig: undefined __thiscall rotate(PlayerEgo * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerEgo::rotate(float, float, float) */

void __thiscall PlayerEgo::rotate(PlayerEgo *this,float param_1,float param_2,float param_3)

{
  uint *puVar1;
  float in_r1;
  float in_r2;
  float in_r3;
  int *piVar2;
  float extraout_s1;
  float extraout_s2;
  Matrix aMStack_50 [60];
  int local_14;
  
  piVar2 = *(int **)(DAT_000b1374 + 0xb130e);
  local_14 = *piVar2;
  puVar1 = *(uint **)(DAT_000b1378 + 0xb133a);
  this[0x14].field_18 = (int *)((float)this[0x14].field_18 + in_r1);
  this[0x14].field_1C = (int)((float)this[0x14].field_1C + in_r2);
  this[0x14].field_20 = (int)((float)this[0x14].field_20 + in_r3);
  AbyssEngine::PaintCanvas::TransformGetLocal(*puVar1);
  AbyssEngine::AEMath::MatrixSetRotation
            (aMStack_50,(float)this[0x14].field_20,extraout_s1,extraout_s2);
  if (*piVar2 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b12fc: push {r4,r5,r7,lr}
  000b12fe: add r7,sp,#0x8
  000b1300: sub sp,#0x48
  000b1302: mov r4,r0
  000b1304: ldr r0,[0x000b1374]
  000b1306: vmov s0,r3
  000b130a: add r0,pc
  000b130c: vmov s2,r1
  000b1310: vmov s10,r2
  000b1314: ldr r5,[r0,#0x0]
  000b1316: ldr r0,[r5,#0x0]
  000b1318: str r0,[sp,#0x44]
  000b131a: vldr.32 s4,[r4,#0x2e8]
  000b131e: vldr.32 s8,[r4,#0x2f0]
  000b1322: vldr.32 s6,[r4,#0x2ec]
  000b1326: vadd.f32 s2,s4,s2
  000b132a: vadd.f32 s0,s8,s0
  000b132e: ldr r0,[0x000b1378]
  000b1330: vadd.f32 s4,s6,s10
  000b1334: ldr r1,[r4,#0x4]
  000b1336: add r0,pc
  000b1338: ldr r0,[r0,#0x0]
  000b133a: vstr.32 s2,[r4,#0x2e8]
  000b133e: vstr.32 s4,[r4,#0x2ec]
  000b1342: vstr.32 s0,[r4,#0x2f0]
  000b1346: ldr r1,[r1,#0xc]
  000b1348: ldr r0,[r0,#0x0]
  000b134a: blx 0x000720c4
  000b134e: ldrd r2,r3,[r4,#0x2e8]
  000b1352: mov r1,r0
  000b1354: vldr.32 s0,[r4,#0x2f0]
  000b1358: add r0,sp,#0x8
  000b135a: vstr.32 s0,[sp]
  000b135e: blx 0x00072094
  000b1362: ldr r0,[sp,#0x44]
  000b1364: ldr r1,[r5,#0x0]
  000b1366: subs r0,r1,r0
  000b1368: itt eq
  000b136a: add.eq sp,#0x48
  000b136c: pop.eq {r4,r5,r7,pc}
  000b136e: blx 0x0006e824
```
