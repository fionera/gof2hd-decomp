# Player::heal
elf 0xa42dc body 94
Sig: undefined __thiscall heal(Player * this, float param_1)

## decompile
```c

/* Player::heal(float) */

void __thiscall Player::heal(Player *this,float param_1)

{
  uint uVar1;
  byte bVar2;
  float in_r1;
  int iVar3;
  uint in_fpscr;
  uint uVar4;
  float fVar5;
  
  fVar5 = *(float *)(this + 0x110) + in_r1;
  *(float *)(this + 0x110) = fVar5;
  uVar1 = in_fpscr & 0xfffffff | (uint)(fVar5 < 1.0) << 0x1f | (uint)(fVar5 == 1.0) << 0x1e;
  uVar4 = uVar1 | (uint)NAN(fVar5) << 0x1c;
  bVar2 = (byte)(uVar1 >> 0x18);
  if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar4 >> 0x1c) & 1)) {
    for (iVar3 = 0; iVar3 < (int)fVar5; iVar3 = iVar3 + 1) {
      regenerateHull(this);
    }
    fVar5 = (float)VectorSignedToFloat((int)fVar5,(byte)(uVar4 >> 0x16) & 3);
    *(float *)(this + 0x110) = *(float *)(this + 0x110) - fVar5;
  }
  return;
}

```
## target disasm
```
  000b42dc: push {r4,r5,r6,r7,lr}
  000b42de: add r7,sp,#0xc
  000b42e0: push.w r11
  000b42e4: vpush {d8}
  000b42e8: vmov s0,r1
  000b42ec: vldr.32 s2,[r0,#0x110]
  000b42f0: mov r4,r0
  000b42f2: vadd.f32 s0,s2,s0
  000b42f6: vmov.f32 s2,0x3f800000
  000b42fa: vstr.32 s0,[r0,#0x110]
  000b42fe: vcmpe.f32 s0,s2
  000b4302: vmrs apsr,fpscr
  000b4306: ble 0x000b4330
  000b4308: vcvt.s32.f32 s16,s0
  000b430c: movs r6,#0x0
  000b430e: vmov r5,s16
  000b4312: b 0x000b431c
  000b4314: mov r0,r4
  000b4316: blx 0x0007291c
  000b431a: adds r6,#0x1
  000b431c: cmp r6,r5
  000b431e: blt 0x000b4314
  000b4320: vcvt.f32.s32 s0,s16
  000b4324: vldr.32 s2,[r4,#0x110]
  000b4328: vsub.f32 s0,s2,s0
  000b432c: vstr.32 s0,[r4,#0x110]
  000b4330: vpop {d8}
  000b4334: pop.w r11
  000b4338: pop {r4,r5,r6,r7,pc}
```
