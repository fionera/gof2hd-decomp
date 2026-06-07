# WantedWindow::update
elf 0xe2028 body 218
Sig: undefined __thiscall update(WantedWindow * this, int param_1)

## decompile
```c

/* WantedWindow::update(int) */

void __thiscall WantedWindow::update(WantedWindow *this,int param_1)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  
  if (this[0x14] != (WantedWindow)0x0) {
    (*(code *)(DAT_001ac284 + 0x1ac288))(*(undefined4 *)(this + 4),param_1);
    return;
  }
  TouchButton::setAlwaysPressed(*(TouchButton **)(*(int *)(*(int *)(this + 0xc) + 4) + 4),true);
  ScrollTouchWindow::update(*(int *)(this + 0x2c));
  if (this[0xa0] == (WantedWindow)0x0) {
    fVar5 = *(float *)(this + 0x94) * *(float *)(this + 0x98);
    fVar6 = -(*(float *)(this + 0x94) * *(float *)(this + 0x98));
    if (0.0 < fVar5) {
      fVar6 = fVar5;
    }
    *(float *)(this + 0x98) = fVar5;
    uVar1 = in_fpscr & 0xfffffff | (uint)(fVar6 < 1.0) << 0x1f | (uint)(fVar6 == 1.0) << 0x1e;
    in_fpscr = uVar1 | (uint)NAN(fVar6) << 0x1c;
    bVar2 = (byte)(uVar1 >> 0x18);
    if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
      fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x84),(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x84) = (int)(fVar5 + fVar6);
    }
  }
  iVar3 = *(int *)(this + 0x84);
  if (0 < iVar3) {
    fVar6 = (float)VectorSignedToFloat(-iVar3,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined4 *)(this + 0x94) = 0x3f800000;
    *(float *)(this + 0x98) = fVar6 * 0.5;
  }
  iVar4 = *(int *)(this + 0xa8) - *(int *)(this + 0xa4);
  if (iVar4 < 0) {
    if (iVar3 < iVar4) {
      fVar6 = (float)VectorSignedToFloat(iVar4 - iVar3,(byte)(in_fpscr >> 0x16) & 3);
      *(undefined4 *)(this + 0x94) = 0x3f800000;
      *(float *)(this + 0x98) = fVar6 * 0.5;
      return;
    }
    return;
  }
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  return;
}

```

## target disasm
```
  000f2028: push {r4,r5,r7,lr}
  000f202a: add r7,sp,#0x8
  000f202c: mov r4,r0
  000f202e: ldrb r0,[r0,#0x14]
  000f2030: mov r5,r1
  000f2032: cbz r0,0x000f2040
  000f2034: ldr r0,[r4,#0x4]
  000f2036: mov r1,r5
  000f2038: pop.w {r4,r5,r7,lr}
  000f203c: b.w 0x001ac278
  000f2040: ldr r0,[r4,#0xc]
  000f2042: movs r1,#0x1
  000f2044: ldr r0,[r0,#0x4]
  000f2046: ldr r0,[r0,#0x4]
  000f2048: blx 0x00075acc
  000f204c: ldr r0,[r4,#0x2c]
  000f204e: mov r1,r5
  000f2050: blx 0x00075b98
  000f2054: ldrb.w r0,[r4,#0xa0]
  000f2058: cbnz r0,0x000f209e
  000f205a: vldr.32 s2,[r4,#0x94]
  000f205e: vldr.32 s4,[r4,#0x98]
  000f2062: vmul.f32 s0,s2,s4
  000f2066: vnmul.f32 s2,s2,s4
  000f206a: vmov.f32 s4,0x3f800000
  000f206e: vcmpe.f32 s0,#0
  000f2072: vmrs apsr,fpscr
  000f2076: it gt
  000f2078: vmov.gt.f32 s2,s0
  000f207c: vstr.32 s0,[r4,#0x98]
  000f2080: vcmpe.f32 s2,s4
  000f2084: vmrs apsr,fpscr
  000f2088: ble 0x000f209e
  000f208a: vldr.32 s2,[r4,#0x84]
  000f208e: vcvt.f32.s32 s2,s2
  000f2092: vadd.f32 s0,s0,s2
  000f2096: vcvt.s32.f32 s0,s0
  000f209a: vstr.32 s0,[r4,#0x84]
  000f209e: ldr.w r0,[r4,#0x84]
  000f20a2: cmp r0,#0x1
  000f20a4: blt 0x000f20c4
  000f20a6: rsbs r1,r0
  000f20a8: vmov.f32 s0,0x3f000000
  000f20ac: vmov s2,r1
  000f20b0: mov.w r1,#0x3f800000
  000f20b4: vcvt.f32.s32 s2,s2
  000f20b8: str.w r1,[r4,#0x94]
  000f20bc: vmul.f32 s0,s2,s0
  000f20c0: vstr.32 s0,[r4,#0x98]
  000f20c4: ldrd r1,r2,[r4,#0xa4]
  000f20c8: subs r1,r2,r1
  000f20ca: cmp.w r1,#0xffffffff
  000f20ce: ble 0x000f20dc
  000f20d0: movs r0,#0x0
  000f20d2: str.w r0,[r4,#0x98]
  000f20d6: str.w r0,[r4,#0x84]
  000f20da: pop {r4,r5,r7,pc}
  000f20dc: cmp r1,r0
  000f20de: it le
  000f20e0: pop.le {r4,r5,r7,pc}
  000f20e2: subs r0,r1,r0
  000f20e4: vmov.f32 s0,0x3f000000
  000f20e8: vmov s2,r0
  000f20ec: mov.w r0,#0x3f800000
  000f20f0: vcvt.f32.s32 s2,s2
  000f20f4: str.w r0,[r4,#0x94]
  000f20f8: vmul.f32 s0,s2,s0
  000f20fc: vstr.32 s0,[r4,#0x98]
  000f2100: pop {r4,r5,r7,pc}
```
