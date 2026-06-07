# BoundingAAB::outerCollide
elf 0x981d2 body 158
Sig: undefined __thiscall outerCollide(BoundingAAB * this, float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingAAB::outerCollide(float, float, float) */

undefined4 __thiscall
BoundingAAB::outerCollide(BoundingAAB *this,float param_1,float param_2,float param_3)

{
  undefined4 uVar1;
  float in_r1;
  float in_r2;
  float in_r3;
  
  uVar1 = 0;
  if ((int)((uint)((*(float *)(this + 8) + *(float *)(this + 0x14)) - *(float *)(this + 0x20) <
                  in_r1) << 0x1f) < 0) {
    if (in_r1 < *(float *)(this + 8) + *(float *)(this + 0x14) + *(float *)(this + 0x20)) {
      uVar1 = 0;
      if ((int)((uint)((*(float *)(this + 0xc) + *(float *)(this + 0x18)) - *(float *)(this + 0x24)
                      < in_r2) << 0x1f) < 0) {
        if (in_r2 < *(float *)(this + 0xc) + *(float *)(this + 0x18) + *(float *)(this + 0x24)) {
          uVar1 = 0;
          if (-1 < (int)((uint)((*(float *)(this + 0x10) + *(float *)(this + 0x1c)) -
                                *(float *)(this + 0x28) < in_r3) << 0x1f)) {
            return 0;
          }
          if (in_r3 < *(float *)(this + 0x10) + *(float *)(this + 0x1c) + *(float *)(this + 0x28)) {
            uVar1 = 1;
          }
        }
      }
    }
  }
  return uVar1;
}

```

## target disasm
```
  000a81d2: vldr.32 s2,[r0,#0x8]
  000a81d6: mov r12,r0
  000a81d8: vldr.32 s4,[r0,#0x14]
  000a81dc: vldr.32 s0,[r0,#0x20]
  000a81e0: movs r0,#0x0
  000a81e2: vadd.f32 s2,s2,s4
  000a81e6: vmov s4,r1
  000a81ea: vsub.f32 s6,s2,s0
  000a81ee: vcmpe.f32 s6,s4
  000a81f2: vmrs apsr,fpscr
  000a81f6: bpl 0x000a826e
  000a81f8: vadd.f32 s0,s2,s0
  000a81fc: vcmpe.f32 s0,s4
  000a8200: vmrs apsr,fpscr
  000a8204: ble 0x000a826e
  000a8206: vldr.32 s4,[r12,#0xc]
  000a820a: vmov s0,r2
  000a820e: vldr.32 s6,[r12,#0x18]
  000a8212: movs r0,#0x0
  000a8214: vldr.32 s2,[r12,#0x24]
  000a8218: vadd.f32 s4,s4,s6
  000a821c: vsub.f32 s6,s4,s2
  000a8220: vcmpe.f32 s6,s0
  000a8224: vmrs apsr,fpscr
  000a8228: bpl 0x000a826e
  000a822a: vadd.f32 s2,s4,s2
  000a822e: vcmpe.f32 s2,s0
  000a8232: vmrs apsr,fpscr
  000a8236: ble 0x000a826e
  000a8238: vldr.32 s4,[r12,#0x10]
  000a823c: vmov s0,r3
  000a8240: vldr.32 s6,[r12,#0x1c]
  000a8244: movs r0,#0x0
  000a8246: vldr.32 s2,[r12,#0x28]
  000a824a: vadd.f32 s4,s4,s6
  000a824e: vsub.f32 s6,s4,s2
  000a8252: vcmpe.f32 s6,s0
  000a8256: vmrs apsr,fpscr
  000a825a: it pl
  000a825c: bx.pl lr
  000a825e: vadd.f32 s2,s4,s2
  000a8262: vcmpe.f32 s2,s0
  000a8266: vmrs apsr,fpscr
  000a826a: it gt
  000a826c: mov.gt r0,#0x1
  000a826e: bx lr
```
