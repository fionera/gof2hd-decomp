# BoundingSphere::BoundingSphere
elf 0x151d28 body 54
Sig: undefined __thiscall BoundingSphere(BoundingSphere * this, float param_1, float param_2, float param_3, float param_4, float param_5, float param_6, float param_7)

## decompile
```c

/* BoundingSphere::BoundingSphere(float, float, float, float, float, float, float) */

void __thiscall
BoundingSphere::BoundingSphere
          (BoundingSphere *this,float param_1,float param_2,float param_3,float param_4,
          float param_5,float param_6,float param_7)

{
  int *piVar1;
  int *piVar2;
  float in_stack_00000000;
  float in_stack_00000004;
  float in_stack_00000008;
  int in_stack_0000000c;
  
  piVar1 = (int *)BoundingVolume::BoundingVolume
                            ((BoundingVolume *)this,in_stack_00000000,param_2,in_stack_00000004,
                             param_4,in_stack_00000008,param_6);
  piVar2 = (int *)(DAT_00161d60 + 0x161d52);
  piVar1[0xe] = in_stack_0000000c;
  *piVar1 = *piVar2 + 8;
  return;
}

```

## target disasm
```
  00161d28: push {r3,r4,r5,r6,r7,lr}
  00161d2a: add r7,sp,#0x10
  00161d2c: vldr.32 s0,[r7,#0x8]
  00161d30: vldr.32 s2,[r7,#0xc]
  00161d34: vldr.32 s4,[r7,#0x10]
  00161d38: vstr.32 s0,[sp]
  00161d3c: vstr.32 s2,[sp,#0x4]
  00161d40: vstr.32 s4,[sp,#0x8]
  00161d44: blx 0x00071f5c
  00161d48: ldr r1,[0x00161d60]
  00161d4a: vldr.32 s0,[r7,#0x14]
  00161d4e: add r1,pc
  00161d50: vstr.32 s0,[r0,#0x38]
  00161d54: ldr r1,[r1,#0x0]
  00161d56: adds r1,#0x8
  00161d58: str r1,[r0,#0x0]
  00161d5a: add sp,#0x10
  00161d5c: pop {r7,pc}
```
