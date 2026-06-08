# TractorBeam::TractorBeam
elf 0x1511b4 body 72
Sig: undefined __thiscall TractorBeam(TractorBeam * this, AEGeometry * param_1, int param_2)

## decompile
```c

/* TractorBeam::TractorBeam(AEGeometry*, int) */

TractorBeam * __thiscall TractorBeam::TractorBeam(TractorBeam *this,AEGeometry *param_1,int param_2)

{
  AEGeometry *this_00;
  
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined2 *)(this + 0x10) = 0;
  this_00 = operator_new(0xc0);
  AEGeometry::AEGeometry
            (this_00,(short)param_2 + 0x3798,
             (PaintCanvas *)**(undefined4 **)(DAT_0016120c + 0x1611e2),false);
  *(AEGeometry **)(this + 0x14) = this_00;
  *(undefined4 *)(this + 0x18) = 0;
  return this;
}

```

## target disasm
```
  001611b4: push {r4,r5,r6,r7,lr}
  001611b6: add r7,sp,#0xc
  001611b8: push.w r8
  001611bc: vmov.i32 q8,#0x0
  001611c0: mov r4,r0
  001611c2: mov.w r8,#0x0
  001611c6: mov r6,r2
  001611c8: vst1.32 {d16,d17},[r0]!
  001611cc: strh.w r8,[r0,#0x0]
  001611d0: movs r0,#0xc0
  001611d2: blx 0x0006eb24
  001611d6: mov r5,r0
  001611d8: ldr r0,[0x0016120c]
  001611da: movw r1,#0x3798
  001611de: add r0,pc
  001611e0: add r1,r6
  001611e2: ldr r0,[r0,#0x0]
  001611e4: uxth r1,r1
  001611e6: ldr r2,[r0,#0x0]
  001611e8: mov r0,r5
  001611ea: movs r3,#0x0
  001611ec: blx 0x0007207c
  001611f0: strd r5,r8,[r4,#0x14]
  001611f4: mov r0,r4
  001611f6: pop.w r8
  001611fa: pop {r4,r5,r6,r7,pc}
```
