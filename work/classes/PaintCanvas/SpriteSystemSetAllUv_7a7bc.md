# PaintCanvas::SpriteSystemSetAllUv
elf 0x7a7bc body 60
Sig: undefined __thiscall SpriteSystemSetAllUv(PaintCanvas * this, uint param_1, float param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemSetAllUv(unsigned int, float, float, float, float) */

void __thiscall
AbyssEngine::PaintCanvas::SpriteSystemSetAllUv
          (PaintCanvas *this,uint param_1,float param_2,float param_3,float param_4,float param_5)

{
  SpriteSystem *in_r2;
  float in_stack_00000000;
  float in_stack_00000004;
  
  if (*(uint *)(this + 0x180) <= param_1) {
    return;
  }
  if (*(int *)(*(int *)(this + 0x184) + param_1 * 4) == 0) {
    return;
  }
  AbyssEngine::SpriteSystemSetAllUv(in_stack_00000004,param_3,in_stack_00000000,param_5,in_r2);
  return;
}

```

## target disasm
```
  0008a7bc: ldr.w r12,[r0,#0x180]
  0008a7c0: cmp r12,r1
  0008a7c2: bls 0x0008a7f6
  0008a7c4: ldr.w r0,[r0,#0x184]
  0008a7c8: ldr.w r0,[r0,r1,lsl #0x2]
  0008a7cc: cmp r0,#0x0
  0008a7ce: it eq
  0008a7d0: bx.eq lr
  0008a7d2: push {r5,r6,r7,lr}
  0008a7d4: add r7,sp,#0x8
  0008a7d6: vldr.32 s0,[r7,#0xc]
  0008a7da: mov r1,r3
  0008a7dc: vldr.32 s2,[r7,#0x8]
  0008a7e0: vmov lr,s0
  0008a7e4: str r0,[sp,#0x0]
  0008a7e6: vmov r12,s2
  0008a7ea: mov r0,r2
  0008a7ec: mov r3,lr
  0008a7ee: mov r2,r12
  0008a7f0: blx 0x0006fcc4
  0008a7f4: pop {r2,r3,r7,pc}
  0008a7f6: bx lr
```
