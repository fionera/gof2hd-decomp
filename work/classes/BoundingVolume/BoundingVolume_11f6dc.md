# BoundingVolume::BoundingVolume
elf 0x11f6dc body 54
Sig: undefined __thiscall BoundingVolume(BoundingVolume * this, float param_1, float param_2, float param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* BoundingVolume::BoundingVolume(float, float, float, float, float, float) */

void __thiscall
BoundingVolume::BoundingVolume
          (BoundingVolume *this,float param_1,float param_2,float param_3,float param_4,
          float param_5,float param_6)

{
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  int iVar1;
  undefined4 in_stack_00000000;
  undefined4 in_stack_00000004;
  undefined4 in_stack_00000008;
  
  iVar1 = *(int *)(DAT_0012f714 + 0x12f6ee);
  *(undefined4 *)(this + 0x14) = in_stack_00000000;
  *(undefined4 *)(this + 0x18) = in_stack_00000004;
  *(undefined4 *)(this + 0x1c) = in_stack_00000008;
  *(int *)this = iVar1 + 8;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = in_r1;
  *(undefined4 *)(this + 0xc) = in_r2;
  *(undefined4 *)(this + 0x10) = in_r3;
  return;
}

```

## target disasm
```
  0012f6dc: push {r4,r6,r7,lr}
  0012f6de: add r7,sp,#0x8
  0012f6e0: ldr r4,[0x0012f714]
  0012f6e2: mov.w r12,#0x0
  0012f6e6: vldr.32 s4,[r7,#0x8]
  0012f6ea: add r4,pc
  0012f6ec: vldr.32 s0,[r7,#0xc]
  0012f6f0: vldr.32 s2,[r7,#0x10]
  0012f6f4: ldr r4,[r4,#0x0]
  0012f6f6: vstr.32 s4,[r0,#0x14]
  0012f6fa: vstr.32 s0,[r0,#0x18]
  0012f6fe: adds r4,#0x8
  0012f700: vstr.32 s2,[r0,#0x1c]
  0012f704: strd r4,r12,[r0,#0x0]
  0012f708: add.w r12,r0,#0x8
  0012f70c: stm.w r12!,{r1,r2,r3}
  0012f710: pop {r4,r6,r7,pc}
```
