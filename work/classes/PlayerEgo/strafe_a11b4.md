# PlayerEgo::strafe
elf 0xa11b4 body 198
Sig: undefined __thiscall strafe(PlayerEgo * this, int param_1, bool param_2)

## decompile
```c

/* PlayerEgo::strafe(int, bool) */

void __thiscall PlayerEgo::strafe(PlayerEgo *this,int param_1,bool param_2)

{
  undefined4 uVar1;
  uint in_fpscr;
  float fVar2;
  int *piVar3;
  undefined8 in_d0;
  undefined4 extraout_s1;
  undefined4 uVar5;
  undefined8 uVar4;
  undefined8 in_d1;
  undefined4 extraout_s3;
  undefined8 uVar6;
  undefined4 in_s5;
  undefined4 extraout_s5;
  undefined4 in_s9;
  undefined4 extraout_s9;
  float fVar7;
  
  uVar1 = (undefined4)((ulonglong)in_d1 >> 0x20);
  if (this[0xb].field_8 == (int *)0x0) {
    if (*(char *)((int)&this[0xf].field_18 + 1) == '\0') {
      uVar5 = (undefined4)((ulonglong)in_d0 >> 0x20);
      piVar3 = this[9].field_10;
    }
    else {
      Status::getShip();
      uVar1 = Ship::getCurrentLoad();
      fVar7 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      Status::getShip();
      uVar1 = Ship::getMaxLoad();
      fVar2 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      piVar3 = (int *)((float)this[9].field_10 * (1.0 - fVar7 / fVar2) * DAT_000b127c +
                      (float)this[9].field_10 * DAT_000b1280);
      in_s5 = extraout_s5;
      in_s9 = extraout_s9;
      uVar5 = extraout_s1;
      uVar1 = extraout_s3;
    }
    fVar2 = -1.0;
    if (param_2) {
      fVar2 = 1.0;
    }
    uVar4 = FloatVectorMin(CONCAT44(uVar5,(float)piVar3 * 30.0 * DAT_000b1284),
                           CONCAT44(in_s9,0x40000000),2,0x20);
    uVar6 = FloatVectorMin(CONCAT44(in_s5,(float)this[0x18].field_20 * 1.5),
                           CONCAT44(uVar1,0x3f800000),2,0x20);
    this[0x18].field_1C = (int)((float)this[0x18].field_20 * fVar2 * (float)uVar4);
    this[0x18].field_20 = (int)uVar6;
  }
  return;
}

```

## target disasm
```
  000b11b4: push {r4,r5,r6,r7,lr}
  000b11b6: add r7,sp,#0xc
  000b11b8: push.w r11
  000b11bc: vpush {d8}
  000b11c0: mov r4,r0
  000b11c2: ldr.w r0,[r0,#0x194]
  000b11c6: mov r5,r2
  000b11c8: cmp r0,#0x0
  000b11ca: bne 0x000b1270
  000b11cc: ldrb.w r0,[r4,#0x235]
  000b11d0: cbz r0,0x000b1224
  000b11d2: ldr r0,[0x000b1288]
  000b11d4: add r0,pc
  000b11d6: ldr r6,[r0,#0x0]
  000b11d8: ldr r0,[r6,#0x0]
  000b11da: blx 0x00071a58
  000b11de: blx 0x00072bf8
  000b11e2: ldr r1,[r6,#0x0]
  000b11e4: vmov s0,r0
  000b11e8: vcvt.f32.s32 s16,s0
  000b11ec: mov r0,r1
  000b11ee: blx 0x00071a58
  000b11f2: blx 0x00072c04
  000b11f6: vmov s0,r0
  000b11fa: vldr.32 s4,[pc,#0x80]
  000b11fe: vmov.f32 s2,0x3f800000
  000b1202: vcvt.f32.s32 s0,s0
  000b1206: vdiv.f32 s0,s16,s0
  000b120a: vsub.f32 s0,s2,s0
  000b120e: vldr.32 s2,[r4,#0x154]
  000b1212: vmul.f32 s0,s2,s0
  000b1216: vmul.f32 s0,s0,s4
  000b121a: vldr.32 s4,[pc,#0x64]
  000b121e: vmla.f32 s0,s2,s4
  000b1222: b 0x000b1228
  000b1224: vldr.32 s0,[r4,#0x154]
  000b1228: vmov.f32 s2,0x41f00000
  000b122c: vldr.32 s6,[pc,#0x54]
  000b1230: vmov.f32 s8,0x40000000
  000b1234: cmp r5,#0x0
  000b1236: vmov.f32 s4,0xbf800000
  000b123a: vmul.f32 s0,s0,s2
  000b123e: vmov.f32 s2,0x3f800000
  000b1242: vmul.f32 s0,s0,s6
  000b1246: it ne
  000b1248: vmov.ne.f32 s4,s2
  000b124c: vldr.32 s6,[r4,#0x380]
  000b1250: vmin.f32 d0,d0,d4
  000b1254: vmul.f32 s0,s4,s0
  000b1258: vmov.f32 s4,0x3fc00000
  000b125c: vmul.f32 s0,s6,s0
  000b1260: vmul.f32 s4,s6,s4
  000b1264: vmin.f32 d1,d2,d1
  000b1268: vstr.32 s0,[r4,#0x37c]
  000b126c: vstr.32 s2,[r4,#0x380]
  000b1270: vpop {d8}
  000b1274: pop.w r11
  000b1278: pop {r4,r5,r6,r7,pc}
```
