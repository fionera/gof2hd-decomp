# Galaxy::distancePercent
elf 0x175f44 body 54
Sig: undefined __thiscall distancePercent(Galaxy * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* Galaxy::distancePercent(int, int, int, int) */

int __thiscall Galaxy::distancePercent(Galaxy *this,int param_1,int param_2,int param_3,int param_4)

{
  float extraout_r0;
  uint in_fpscr;
  undefined4 uVar1;
  undefined8 in_d0;
  
  uVar1 = VectorSignedToFloat((param_4 - param_2) * (param_4 - param_2) +
                              (param_3 - param_1) * (param_3 - param_1),(byte)(in_fpscr >> 0x16) & 3
                             );
  Globals::sqrt((Globals *)**(undefined4 **)(DAT_00185f7c + 0x185f62),
                (double)CONCAT44((int)((ulonglong)in_d0 >> 0x20),uVar1));
  return (int)extraout_r0;
}

```

## target disasm
```
  00185f44: push {r7,lr}
  00185f46: mov r7,sp
  00185f48: subs r1,r3,r1
  00185f4a: ldr r0,[r7,#0x8]
  00185f4c: muls r1,r1
  00185f4e: subs r0,r0,r2
  00185f50: mla r0,r0,r0,r1
  00185f54: vmov s0,r0
  00185f58: vcvt.f32.s32 s0,s0
  00185f5c: ldr r0,[0x00185f7c]
  00185f5e: add r0,pc
  00185f60: ldr r0,[r0,#0x0]
  00185f62: vmov r1,s0
  00185f66: ldr r0,[r0,#0x0]
  00185f68: blx 0x000760e4
  00185f6c: vmov s0,r0
  00185f70: vcvt.s32.f32 s0,s0
  00185f74: vmov r0,s0
  00185f78: pop {r7,pc}
```
