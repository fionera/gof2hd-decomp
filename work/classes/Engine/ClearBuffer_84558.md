# Engine::ClearBuffer
elf 0x84558 body 118
Sig: undefined __thiscall ClearBuffer(Engine * this, uint param_1)

## decompile
```c

/* AbyssEngine::Engine::ClearBuffer(unsigned int) */

void __thiscall AbyssEngine::Engine::ClearBuffer(Engine *this,uint param_1)

{
  uint in_fpscr;
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  
  dVar1 = (double)VectorUnsignedToFloat(param_1 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
  dVar2 = (double)VectorUnsignedToFloat((param_1 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
  dVar3 = (double)VectorUnsignedToFloat((param_1 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
  dVar4 = (double)VectorUnsignedToFloat(param_1 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
  glClearColor((float)(dVar1 / DAT_000945d0),(float)(dVar2 / DAT_000945d0),
               (float)(dVar3 / DAT_000945d0),(float)(dVar4 / DAT_000945d0));
  (*(code *)(DAT_001ab094 + 0x1ab098))(0x4100);
  return;
}

```

## target disasm
```
  00094558: push {r7,lr}
  0009455a: mov r7,sp
  0009455c: lsrs r0,r1,#0x18
  0009455e: vldr.64 d19,[pc,#0x70]
  00094562: vmov s0,r0
  00094566: vcvt.f64.u32 d16,s0
  0009456a: ubfx r0,r1,#0x10,#0x8
  0009456e: vmov s0,r0
  00094572: vdiv.f64 d16,d16,d19
  00094576: vcvt.f64.u32 d17,s0
  0009457a: ubfx r0,r1,#0x8,#0x8
  0009457e: vmov s0,r0
  00094582: uxtb r0,r1
  00094584: vdiv.f64 d17,d17,d19
  00094588: vcvt.f64.u32 d18,s0
  0009458c: vmov s0,r0
  00094590: vdiv.f64 d18,d18,d19
  00094594: vcvt.f64.u32 d20,s0
  00094598: vdiv.f64 d19,d20,d19
  0009459c: vcvt.f32.f64 s0,d16
  000945a0: vcvt.f32.f64 s2,d17
  000945a4: vcvt.f32.f64 s4,d18
  000945a8: vcvt.f32.f64 s6,d19
  000945ac: vmov r0,s0
  000945b0: vmov r1,s2
  000945b4: vmov r2,s4
  000945b8: vmov r3,s6
  000945bc: blx 0x0006eecc
  000945c0: mov.w r0,#0x4100
  000945c4: pop.w {r7,lr}
  000945c8: b.w 0x001ab088
  001ab088: bx pc
```
