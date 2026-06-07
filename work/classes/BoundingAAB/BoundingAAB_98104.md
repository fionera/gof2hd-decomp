# BoundingAAB::BoundingAAB
elf 0x98104 body 144
Sig: undefined __thiscall BoundingAAB(BoundingAAB * this, float param_1, float param_2, float param_3, float param_4, float param_5, float param_6, float param_7, float param_8, float param_9)

## decompile
```c

/* BoundingAAB::BoundingAAB(float, float, float, float, float, float, float, float, float) */

void __thiscall
BoundingAAB::BoundingAAB
          (BoundingAAB *this,float param_1,float param_2,float param_3,float param_4,float param_5,
          float param_6,float param_7,float param_8,float param_9)

{
  int *piVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float in_stack_00000000;
  float in_stack_00000004;
  float in_stack_00000008;
  float in_stack_0000000c;
  float in_stack_00000010;
  float in_stack_00000014;
  
  piVar1 = (int *)BoundingVolume::BoundingVolume
                            ((BoundingVolume *)this,in_stack_00000000,param_2,in_stack_00000004,
                             param_4,in_stack_00000008,param_6);
  fVar3 = in_stack_0000000c * -0.5;
  if (0.0 < in_stack_0000000c * 0.5) {
    fVar3 = in_stack_0000000c * 0.5;
  }
  fVar4 = in_stack_00000010 * -0.5;
  if (0.0 < in_stack_00000010 * 0.5) {
    fVar4 = in_stack_00000010 * 0.5;
  }
  fVar2 = in_stack_00000014 * -0.5;
  if (0.0 < in_stack_00000014 * 0.5) {
    fVar2 = in_stack_00000014 * 0.5;
  }
  *piVar1 = *(int *)(DAT_000a8194 + 0xa813e) + 8;
  piVar1[8] = (int)fVar3;
  piVar1[9] = (int)fVar4;
  piVar1[10] = (int)fVar2;
  return;
}

```

## target disasm
```
  000a8104: push {r3,r4,r5,r6,r7,lr}
  000a8106: add r7,sp,#0x10
  000a8108: vldr.32 s0,[r7,#0x8]
  000a810c: vldr.32 s2,[r7,#0xc]
  000a8110: vldr.32 s4,[r7,#0x10]
  000a8114: vstr.32 s0,[sp]
  000a8118: vstr.32 s2,[sp,#0x4]
  000a811c: vstr.32 s4,[sp,#0x8]
  000a8120: blx 0x00071f5c
  000a8124: vmov.f32 s0,0x3f000000
  000a8128: vldr.32 s4,[r7,#0x14]
  000a812c: vldr.32 s6,[r7,#0x18]
  000a8130: vmov.f32 s2,0xbf000000
  000a8134: ldr r1,[0x000a8194]
  000a8136: vldr.32 s10,[r7,#0x1c]
  000a813a: add r1,pc
  000a813c: ldr r1,[r1,#0x0]
  000a813e: vmul.f32 s8,s4,s0
  000a8142: vmul.f32 s12,s6,s0
  000a8146: adds r1,#0x8
  000a8148: vmul.f32 s4,s4,s2
  000a814c: vmul.f32 s0,s10,s0
  000a8150: vmul.f32 s6,s6,s2
  000a8154: vmul.f32 s2,s10,s2
  000a8158: vcmpe.f32 s8,#0
  000a815c: vmrs apsr,fpscr
  000a8160: vcmpe.f32 s12,#0
  000a8164: it gt
  000a8166: vmov.gt.f32 s4,s8
  000a816a: vmrs apsr,fpscr
  000a816e: vcmpe.f32 s0,#0
  000a8172: it gt
  000a8174: vmov.gt.f32 s6,s12
  000a8178: vmrs apsr,fpscr
  000a817c: it gt
  000a817e: vmov.gt.f32 s2,s0
  000a8182: str r1,[r0,#0x0]
  000a8184: vstr.32 s4,[r0,#0x20]
  000a8188: vstr.32 s6,[r0,#0x24]
  000a818c: vstr.32 s2,[r0,#0x28]
  000a8190: add sp,#0x10
  000a8192: pop {r7,pc}
```
