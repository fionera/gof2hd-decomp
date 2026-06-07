# LODManager::forceUpdate
elf 0x952c4 body 178
Sig: undefined __thiscall forceUpdate(LODManager * this, int param_1, bool param_2)

## decompile
```c

/* LODManager::forceUpdate(int, bool) */

void __thiscall LODManager::forceUpdate(LODManager *this,int param_1,bool param_2)

{
  int *piVar1;
  uint uVar2;
  AEGeometry *this_00;
  float fVar3;
  undefined8 in_d6;
  undefined4 in_s14;
  undefined8 local_40;
  undefined4 local_38;
  int local_34;
  
  piVar1 = *(int **)(DAT_000a5378 + 0xa52de);
  _local_38 = CONCAT44(*piVar1,in_s14);
  uVar2 = **(uint **)(DAT_000a5380 + 0xa52e4);
  local_40 = in_d6;
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  AbyssEngine::PaintCanvas::CameraGetLocal(uVar2);
  AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_40);
  fVar3 = (float)AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 4),(Vector *)&local_40);
  uVar2 = 0;
  while( true ) {
    if (**(uint **)this <= uVar2) break;
    this_00 = *(AEGeometry **)((*(uint **)this)[1] + uVar2 * 4);
    if (param_2) {
      fVar3 = (float)AEGeometry::getParentPosition();
    }
    else {
      local_40 = *(undefined8 *)(this + 4);
      _local_38 = CONCAT44(local_34,*(undefined4 *)(this + 0xc));
    }
    fVar3 = (float)AEGeometry::updateLod(this_00,(Vector *)&local_40,fVar3);
    uVar2 = uVar2 + 1;
  }
  if (*piVar1 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000a52c4: push {r4,r5,r6,r7,lr}
  000a52c6: add r7,sp,#0xc
  000a52c8: push {r7,r8,r9,r10,r11}
  000a52cc: vpush {d5,d6,d7,d8}
  000a52d0: mov r10,r0
  000a52d2: ldr r0,[0x000a5378]
  000a52d4: ldr r1,[0x000a537c]
  000a52d6: mov r8,r2
  000a52d8: ldr r2,[0x000a5380]
  000a52da: add r0,pc
  000a52dc: add r1,pc
  000a52de: ldr r3,[r0,#0x0]
  000a52e0: add r2,pc
  000a52e2: ldr r0,[r1,#0x0]
  000a52e4: ldr r1,[r2,#0x0]
  000a52e6: str r3,[sp,#0x4]
  000a52e8: ldr r2,[r3,#0x0]
  000a52ea: vldr.32 s16,[r0,#0x28]
  000a52ee: str r2,[sp,#0x14]
  000a52f0: ldr r4,[r1,#0x0]
  000a52f2: mov r0,r4
  000a52f4: blx 0x000717f4
  000a52f8: mov r1,r0
  000a52fa: mov r0,r4
  000a52fc: blx 0x0006ff1c
  000a5300: add.w r9,sp,#0x8
  000a5304: mov r1,r0
  000a5306: mov r0,r9
  000a5308: blx 0x0006f16c
  000a530c: add.w r4,r10,#0x4
  000a5310: mov r1,r9
  000a5312: mov r0,r4
  000a5314: blx 0x0006eb3c
  000a5318: vmov r6,s16
  000a531c: mov.w r11,#0x0
  000a5320: b 0x000a5352
  000a5322: ldr r0,[r0,#0x4]
  000a5324: cmp.w r8,#0x0
  000a5328: ldr.w r5,[r0,r11,lsl #0x2]
  000a532c: beq 0x000a5338
  000a532e: mov r0,r9
  000a5330: mov r1,r5
  000a5332: blx 0x00071800
  000a5336: b 0x000a5344
  000a5338: vldr.64 d16,[r4]
  000a533c: ldr r0,[r4,#0x8]
  000a533e: str r0,[sp,#0x10]
  000a5340: vstr.64 d16,[sp,#0x8]
  000a5344: mov r0,r5
  000a5346: mov r1,r9
  000a5348: mov r2,r6
  000a534a: blx 0x0007180c
  000a534e: add.w r11,r11,#0x1
  000a5352: ldr.w r0,[r10,#0x0]
  000a5356: ldr r1,[r0,#0x0]
  000a5358: cmp r11,r1
  000a535a: bcc 0x000a5322
  000a535c: ldr r0,[sp,#0x14]
  000a535e: ldr r1,[sp,#0x4]
  000a5360: ldr r1,[r1,#0x0]
  000a5362: subs r0,r1,r0
  000a5364: itttt eq
  000a5366: vpop.eq {d5,d6,d7,d8}
  000a536a: add.eq sp,#0x4
  000a536c: pop.eq.w {r8,r9,r10,r11}
  000a5370: pop.eq {r4,r5,r6,r7,pc}
  000a5372: blx 0x0006e824
```
