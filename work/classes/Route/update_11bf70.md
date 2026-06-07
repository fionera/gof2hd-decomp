# Route::update
elf 0x11bf70 body 252
Sig: undefined __thiscall update(Route * this, float param_1, float param_2, float param_3)

## decompile
```c

/* Route::update(float, float, float) */

float __thiscall Route::update(Route *this,float param_1,float param_2,float param_3)

{
  byte bVar1;
  int iVar2;
  uint *puVar3;
  float in_r1;
  uint uVar4;
  float in_r2;
  float in_r3;
  int iVar5;
  uint uVar6;
  uint in_fpscr;
  float fVar7;
  
  iVar2 = *(int *)this;
  if ((iVar2 < **(int **)(this + 0xc)) &&
     (*(int *)(*(int *)(*(int *)(this + 0x10) + 4) + iVar2 * 4) == 0)) {
    iVar2 = *(int *)((*(int **)(this + 0xc))[1] + iVar2 * 4);
    fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(iVar2 + 0x124),(byte)(in_fpscr >> 0x16) & 3);
    fVar7 = in_r1 - fVar7;
    param_1 = DAT_0012c06c;
    if ((fVar7 < DAT_0012c06c) &&
       (uVar4 = in_fpscr & 0xfffffff | (uint)(fVar7 < DAT_0012c070) << 0x1f |
                (uint)(fVar7 == DAT_0012c070) << 0x1e, bVar1 = (byte)(uVar4 >> 0x18),
       !(bool)(bVar1 >> 6 & 1) && (bool)(bVar1 >> 7) == (NAN(fVar7) || NAN(DAT_0012c070)))) {
      fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(iVar2 + 0x128),(byte)(uVar4 >> 0x16) & 3);
      fVar7 = in_r2 - fVar7;
      if ((fVar7 < DAT_0012c06c) &&
         (uVar4 = in_fpscr & 0xfffffff | (uint)(fVar7 < DAT_0012c070) << 0x1f |
                  (uint)(fVar7 == DAT_0012c070) << 0x1e, bVar1 = (byte)(uVar4 >> 0x18),
         !(bool)(bVar1 >> 6 & 1) && (bool)(bVar1 >> 7) == (NAN(fVar7) || NAN(DAT_0012c070)))) {
        fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(iVar2 + 300),(byte)(uVar4 >> 0x16) & 3);
        if (((int)((uint)(in_r3 - fVar7 < DAT_0012c06c) << 0x1f) < 0) &&
           (DAT_0012c070 < in_r3 - fVar7)) {
          Waypoint::setActive(SUB41(iVar2,0));
          param_1 = (float)Waypoint::reached(*(Waypoint **)
                                              (*(int *)(*(int *)(this + 0xc) + 4) + *(int *)this * 4
                                              ));
          iVar5 = *(int *)this;
          puVar3 = *(uint **)(this + 0xc);
          iVar2 = iVar5 + 1;
          *(int *)this = iVar2;
          uVar4 = *puVar3;
          if ((this[4] != (Route)0x0) && ((int)(uVar4 - 1) <= iVar5)) {
            *(undefined4 *)this = 0;
            for (uVar6 = 0; uVar6 < uVar4; uVar6 = uVar6 + 1) {
              param_1 = (float)Waypoint::reset(*(Waypoint **)(puVar3[1] + uVar6 * 4));
              puVar3 = *(uint **)(this + 0xc);
              uVar4 = *puVar3;
            }
            iVar2 = *(int *)this;
          }
          if (iVar2 < (int)uVar4) {
            fVar7 = (float)(*(code *)(DAT_001ac364 + 0x1ac368))
                                     (*(undefined4 *)(puVar3[1] + iVar2 * 4),1);
            return fVar7;
          }
        }
      }
    }
  }
  return param_1;
}

```

## target disasm
```
  0012bf70: push {r4,r5,r6,r7,lr}
  0012bf72: add r7,sp,#0xc
  0012bf74: push.w r11
  0012bf78: mov r6,r0
  0012bf7a: ldr r0,[r0,#0x0]
  0012bf7c: ldr r5,[r6,#0xc]
  0012bf7e: ldr r4,[r5,#0x0]
  0012bf80: cmp r0,r4
  0012bf82: bge 0x0012bf8e
  0012bf84: ldr r4,[r6,#0x10]
  0012bf86: ldr r4,[r4,#0x4]
  0012bf88: ldr.w r4,[r4,r0,lsl #0x2]
  0012bf8c: cbz r4,0x0012bf94
  0012bf8e: pop.w r11
  0012bf92: pop {r4,r5,r6,r7,pc}
  0012bf94: vmov s0,r1
  0012bf98: ldr r1,[r5,#0x4]
  0012bf9a: ldr.w r0,[r1,r0,lsl #0x2]
  0012bf9e: vldr.32 s2,[r0,#0x124]
  0012bfa2: vcvt.f32.s32 s2,s2
  0012bfa6: vsub.f32 s4,s0,s2
  0012bfaa: vldr.32 s0,[pc,#0xc0]
  0012bfae: vcmpe.f32 s4,s0
  0012bfb2: vmrs apsr,fpscr
  0012bfb6: bpl 0x0012bf8e
  0012bfb8: vldr.32 s2,[pc,#0xb4]
  0012bfbc: vcmpe.f32 s4,s2
  0012bfc0: vmrs apsr,fpscr
  0012bfc4: ble 0x0012bf8e
  0012bfc6: vldr.32 s6,[r0,#0x128]
  0012bfca: vmov s4,r2
  0012bfce: vcvt.f32.s32 s6,s6
  0012bfd2: vsub.f32 s4,s4,s6
  0012bfd6: vcmpe.f32 s4,s0
  0012bfda: vmrs apsr,fpscr
  0012bfde: bpl 0x0012bf8e
  0012bfe0: vcmpe.f32 s4,s2
  0012bfe4: vmrs apsr,fpscr
  0012bfe8: ble 0x0012bf8e
  0012bfea: vldr.32 s6,[r0,#0x12c]
  0012bfee: vmov s4,r3
  0012bff2: vcvt.f32.s32 s6,s6
  0012bff6: vsub.f32 s4,s4,s6
  0012bffa: vcmpe.f32 s4,s0
  0012bffe: vmrs apsr,fpscr
  0012c002: bpl 0x0012bf8e
  0012c004: vcmpe.f32 s4,s2
  0012c008: vmrs apsr,fpscr
  0012c00c: ble 0x0012bf8e
  0012c00e: movs r1,#0x0
  0012c010: blx 0x00075fac
  0012c014: ldr r1,[r6,#0xc]
  0012c016: ldr r0,[r6,#0x0]
  0012c018: ldr r1,[r1,#0x4]
  0012c01a: ldr.w r0,[r1,r0,lsl #0x2]
  0012c01e: blx 0x00074194
  0012c022: ldr r3,[r6,#0x0]
  0012c024: ldr r0,[r6,#0xc]
  0012c026: adds r2,r3,#0x1
  0012c028: str r2,[r6,#0x0]
  0012c02a: ldrb r5,[r6,#0x4]
  0012c02c: ldr r1,[r0,#0x0]
  0012c02e: cbz r5,0x0012c052
  0012c030: subs r5,r1,#0x1
  0012c032: cmp r3,r5
  0012c034: blt 0x0012c052
  0012c036: movs r5,#0x0
  0012c038: str r5,[r6,#0x0]
  0012c03a: b 0x0012c04c
  0012c03c: ldr r0,[r0,#0x4]
  0012c03e: ldr.w r0,[r0,r5,lsl #0x2]
  0012c042: blx 0x00075f94
  0012c046: ldr r0,[r6,#0xc]
  0012c048: adds r5,#0x1
  0012c04a: ldr r1,[r0,#0x0]
  0012c04c: cmp r5,r1
  0012c04e: bcc 0x0012c03c
  0012c050: ldr r2,[r6,#0x0]
  0012c052: cmp r2,r1
  0012c054: bge 0x0012bf8e
  0012c056: ldr r0,[r0,#0x4]
  0012c058: movs r1,#0x1
  0012c05a: ldr.w r0,[r0,r2,lsl #0x2]
  0012c05e: pop.w r11
  0012c062: pop.w {r4,r5,r6,r7,lr}
  0012c066: b.w 0x001ac358
  001ac358: bx pc
```
