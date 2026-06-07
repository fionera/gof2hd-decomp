# Layout::drawMask
elf 0xd3168 body 68
Sig: undefined __thiscall drawMask(Layout * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* Layout::drawMask(int, int, int, int) */

void __thiscall Layout::drawMask(Layout *this,int param_1,int param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  uint *puVar2;
  
  puVar2 = *(uint **)(DAT_000e31ac + 0xe317c);
  uVar1 = AbyssEngine::PaintCanvas::GetColor((PaintCanvas *)*puVar2);
  AbyssEngine::PaintCanvas::SetColor(*puVar2);
  AbyssEngine::PaintCanvas::FillRectangle(*puVar2,param_1,param_2,param_3);
  (*(code *)(DAT_001ac094 + 0x1ac098))(*puVar2,uVar1,param_4,&stack0xfffffff8);
  return;
}

```

## target disasm
```
  000e3168: push {r4,r5,r6,r7,lr}
  000e316a: add r7,sp,#0xc
  000e316c: push {r6,r7,r8,r9,r11}
  000e3170: ldr r0,[0x000e31ac]
  000e3172: mov r8,r3
  000e3174: mov r9,r2
  000e3176: mov r6,r1
  000e3178: add r0,pc
  000e317a: ldr r5,[r0,#0x0]
  000e317c: ldr r0,[r5,#0x0]
  000e317e: blx 0x0006fa90
  000e3182: mov r4,r0
  000e3184: ldr r0,[r5,#0x0]
  000e3186: movs r1,#0x80
  000e3188: blx 0x0006fac0
  000e318c: ldr r0,[r5,#0x0]
  000e318e: mov r2,r9
  000e3190: ldr r1,[r7,#0x8]
  000e3192: mov r3,r8
  000e3194: str r1,[sp,#0x0]
  000e3196: mov r1,r6
  000e3198: blx 0x00074de8
  000e319c: ldr r0,[r5,#0x0]
  000e319e: mov r1,r4
  000e31a0: pop.w {r2,r3,r8,r9,r11}
  000e31a4: pop.w {r4,r5,r6,r7,lr}
  000e31a8: b.w 0x001ac088
```
