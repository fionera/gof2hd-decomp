# NewsTicker::update
elf 0x15e0e4 body 136
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* NewsTicker::update(int) */

void NewsTicker::update(int param_1)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  undefined4 in_r1;
  uint in_fpscr;
  float fVar4;
  undefined4 uVar5;
  float fVar6;
  
  if (*(char *)(param_1 + 0x28) == '\0') {
    fVar4 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
    fVar4 = (fVar4 / DAT_0016e16c) * DAT_0016e170;
    iVar3 = GameText::getLanguage();
    if (iVar3 == 9) {
      fVar4 = fVar4 + *(float *)param_1;
      fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0xc),(byte)(in_fpscr >> 0x16) & 3
                                        );
      *(float *)param_1 = fVar4;
      uVar1 = in_fpscr & 0xfffffff | (uint)(fVar4 < fVar6) << 0x1f | (uint)(fVar4 == fVar6) << 0x1e;
      bVar2 = (byte)(uVar1 >> 0x18);
      if (!(bool)(bVar2 >> 6 & 1) && (bool)(bVar2 >> 7) == (NAN(fVar4) || NAN(fVar6))) {
        uVar5 = VectorSignedToFloat(-*(int *)(param_1 + 0x10),(byte)(uVar1 >> 0x16) & 3);
        *(undefined4 *)param_1 = uVar5;
      }
    }
    else {
      fVar4 = *(float *)param_1 - fVar4;
      fVar6 = (float)VectorSignedToFloat(-*(int *)(param_1 + 0x10),(byte)(in_fpscr >> 0x16) & 3);
      *(float *)param_1 = fVar4;
      if ((int)((uint)(fVar4 < fVar6) << 0x1f) < 0) {
        *(undefined4 *)param_1 = 0;
      }
    }
  }
  return;
}

```

## target disasm
```
  0016e0e4: push {r4,r6,r7,lr}
  0016e0e6: add r7,sp,#0x8
  0016e0e8: vpush {d8}
  0016e0ec: mov r4,r0
  0016e0ee: ldrb.w r0,[r0,#0x28]
  0016e0f2: cbnz r0,0x0016e166
  0016e0f4: vmov s0,r1
  0016e0f8: vldr.32 s2,[pc,#0x70]
  0016e0fc: vcvt.f32.s32 s0,s0
  0016e100: vdiv.f32 s0,s0,s2
  0016e104: vldr.32 s2,[pc,#0x68]
  0016e108: vmul.f32 s16,s0,s2
  0016e10c: blx 0x0006f544
  0016e110: vldr.32 s0,[r4]
  0016e114: cmp r0,#0x9
  0016e116: bne 0x0016e144
  0016e118: vldr.32 s2,[r4,#0xc]
  0016e11c: vadd.f32 s0,s16,s0
  0016e120: vcvt.f32.s32 s2,s2
  0016e124: vstr.32 s0,[r4]
  0016e128: vcmpe.f32 s0,s2
  0016e12c: vmrs apsr,fpscr
  0016e130: ble 0x0016e166
  0016e132: ldr r0,[r4,#0x10]
  0016e134: rsbs r0,r0
  0016e136: vmov s0,r0
  0016e13a: vcvt.f32.s32 s0,s0
  0016e13e: vstr.32 s0,[r4]
  0016e142: b 0x0016e166
  0016e144: ldr r0,[r4,#0x10]
  0016e146: vsub.f32 s0,s0,s16
  0016e14a: rsbs r0,r0
  0016e14c: vmov s2,r0
  0016e150: vcvt.f32.s32 s2,s2
  0016e154: vstr.32 s0,[r4]
  0016e158: vcmpe.f32 s0,s2
  0016e15c: vmrs apsr,fpscr
  0016e160: itt mi
  0016e162: mov.mi r0,#0x0
  0016e164: str.mi r0,[r4,#0x0]
  0016e166: vpop {d8}
  0016e16a: pop {r4,r6,r7,pc}
```
