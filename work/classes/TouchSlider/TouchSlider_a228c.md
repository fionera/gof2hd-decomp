# TouchSlider::TouchSlider
elf 0xa228c body 152
Sig: undefined __thiscall TouchSlider(TouchSlider * this, int param_1, int param_2, int param_3, float param_4)

## decompile
```c

/* TouchSlider::TouchSlider(int, int, int, float) */

TouchSlider * __thiscall
TouchSlider::TouchSlider(TouchSlider *this,int param_1,int param_2,int param_3,float param_4)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  ushort uVar4;
  uint *puVar5;
  undefined4 in_stack_00000000;
  
  puVar2 = (undefined4 *)(DAT_000b2324 + 0xb22a2);
  *(int *)(this + 0x10) = param_1;
  *(undefined4 *)(this + 0x24) = in_stack_00000000;
  puVar5 = (uint *)*puVar2;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar5,0x51a,(uint *)(this + 0x30));
  uVar4 = 0x51b;
  if (param_1 == 1) {
    uVar4 = 0x51c;
  }
  if (param_1 == 0) {
    uVar4 = 0x519;
  }
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar5,uVar4,(uint *)(this + 0x2c));
  uVar3 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar5);
  *(undefined4 *)(this + 0x14) = uVar3;
  uVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar5);
  *(undefined4 *)(this + 0x18) = uVar3;
  uVar3 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar5);
  *(undefined4 *)(this + 0x1c) = uVar3;
  uVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar5);
  *(undefined4 *)(this + 0x20) = uVar3;
  *(undefined2 *)(this + 0x34) = 0;
  setPosition(this,param_2,param_3);
  iVar1 = DAT_000b2328;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x38) = *(undefined4 *)(**(int **)(iVar1 + 0xb2316) + 0x7c);
  return this;
}

```

## target disasm
```
  000b228c: push {r4,r5,r6,r7,lr}
  000b228e: add r7,sp,#0xc
  000b2290: push {r8,r9,r11}
  000b2294: mov r4,r0
  000b2296: ldr r0,[0x000b2324]
  000b2298: vldr.32 s0,[r7,#0x8]
  000b229c: mov r9,r2
  000b229e: add r0,pc
  000b22a0: str r1,[r4,#0x10]
  000b22a2: vstr.32 s0,[r4,#0x24]
  000b22a6: add.w r2,r4,#0x30
  000b22aa: ldr r6,[r0,#0x0]
  000b22ac: mov r5,r1
  000b22ae: movw r1,#0x51a
  000b22b2: mov r8,r3
  000b22b4: ldr r0,[r6,#0x0]
  000b22b6: blx 0x00071cf8
  000b22ba: movw r1,#0x51b
  000b22be: cmp r5,#0x1
  000b22c0: it eq
  000b22c2: movw.eq r1,#0x51c
  000b22c6: cmp r5,#0x0
  000b22c8: it eq
  000b22ca: movw.eq r1,#0x519
  000b22ce: ldr r0,[r6,#0x0]
  000b22d0: add.w r2,r4,#0x2c
  000b22d4: blx 0x00071cf8
  000b22d8: ldr r1,[r4,#0x30]
  000b22da: ldr r0,[r6,#0x0]
  000b22dc: blx 0x00072d84
  000b22e0: ldr r1,[r4,#0x30]
  000b22e2: str r0,[r4,#0x14]
  000b22e4: ldr r0,[r6,#0x0]
  000b22e6: blx 0x00072d90
  000b22ea: ldr r1,[r4,#0x2c]
  000b22ec: str r0,[r4,#0x18]
  000b22ee: ldr r0,[r6,#0x0]
  000b22f0: blx 0x00072d84
  000b22f4: ldr r1,[r4,#0x2c]
  000b22f6: str r0,[r4,#0x1c]
  000b22f8: ldr r0,[r6,#0x0]
  000b22fa: blx 0x00072d90
  000b22fe: str r0,[r4,#0x20]
  000b2300: movs r5,#0x0
  000b2302: mov r0,r4
  000b2304: mov r1,r9
  000b2306: mov r2,r8
  000b2308: strh r5,[r4,#0x34]
  000b230a: blx 0x00072db4
  000b230e: ldr r0,[0x000b2328]
  000b2310: str r5,[r4,#0x28]
  000b2312: add r0,pc
  000b2314: ldr r0,[r0,#0x0]
  000b2316: ldr r0,[r0,#0x0]
  000b2318: ldr r0,[r0,#0x7c]
  000b231a: str r0,[r4,#0x38]
  000b231c: mov r0,r4
  000b231e: pop.w {r8,r9,r11}
  000b2322: pop {r4,r5,r6,r7,pc}
```
