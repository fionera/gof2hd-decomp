# Engine::SetUVMatrix
elf 0x850f0 body 144
Sig: undefined __thiscall SetUVMatrix(Engine * this, Matrix * param_1)

## decompile
```c

/* AbyssEngine::Engine::SetUVMatrix(AbyssEngine::AEMath::Matrix const&) */

void __thiscall AbyssEngine::Engine::SetUVMatrix(Engine *this,Matrix *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  
  if (**(char **)(DAT_00095180 + 0x95102) == '\0') {
    glMatrixMode(0x1702);
    AEMath::MatrixGetGL(param_1,(float *)(this + 0x428));
    glLoadMatrixf(this + 0x428);
    (*(code *)(DAT_001ab644 + 0x1ab648))(0x1700);
    return;
  }
  uVar9 = *(undefined4 *)(param_1 + 4);
  uVar11 = *(undefined4 *)(param_1 + 8);
  uVar1 = *(undefined4 *)(param_1 + 0xc);
  uVar4 = *(undefined4 *)(param_1 + 0x10);
  uVar3 = *(undefined4 *)(param_1 + 0x14);
  uVar8 = *(undefined4 *)(param_1 + 0x18);
  uVar7 = *(undefined4 *)(param_1 + 0x1c);
  uVar6 = *(undefined4 *)(param_1 + 0x20);
  uVar2 = *(undefined4 *)(param_1 + 0x24);
  uVar10 = *(undefined4 *)(param_1 + 0x28);
  uVar5 = *(undefined4 *)(param_1 + 0x2c);
  *(undefined4 *)(this + 0x1c4) = *(undefined4 *)param_1;
  *(undefined4 *)(this + 0x1c8) = uVar4;
  *(undefined4 *)(this + 0x1cc) = uVar6;
  *(undefined4 *)(this + 0x1d0) = 0;
  *(undefined4 *)(this + 0x1d4) = uVar9;
  *(undefined4 *)(this + 0x1d8) = uVar3;
  *(undefined4 *)(this + 0x1dc) = uVar2;
  *(undefined4 *)(this + 0x1e0) = 0;
  *(undefined4 *)(this + 0x1e4) = uVar11;
  *(undefined4 *)(this + 0x1e8) = uVar8;
  *(undefined4 *)(this + 0x1ec) = uVar10;
  *(undefined4 *)(this + 0x1f0) = 0;
  *(undefined4 *)(this + 500) = uVar1;
  *(undefined4 *)(this + 0x1f8) = uVar7;
  *(undefined4 *)(this + 0x1fc) = uVar5;
  *(undefined4 *)(this + 0x200) = 0x3f800000;
  return;
}

```

## target disasm
```
  000950f0: push {r4,r5,r6,r7,lr}
  000950f2: add r7,sp,#0xc
  000950f4: push {r7,r8,r9,r10,r11}
  000950f8: mov r4,r0
  000950fa: ldr r0,[0x00095180]
  000950fc: mov r5,r1
  000950fe: add r0,pc
  00095100: ldr r0,[r0,#0x0]
  00095102: ldrb r0,[r0,#0x0]
  00095104: cbz r0,0x00095156
  00095106: ldm.w r5,{r0,r10,lr}
  0009510a: mov.w r12,#0x0
  0009510e: ldr r1,[r5,#0xc]
  00095110: str r1,[sp,#0x0]
  00095112: ldrd r3,r2,[r5,#0x10]
  00095116: ldrd r9,r8,[r5,#0x18]
  0009511a: ldrd r6,r1,[r5,#0x20]
  0009511e: ldrd r11,r5,[r5,#0x28]
  00095122: str.w r0,[r4,#0x1c4]
  00095126: mov.w r0,#0x3f800000
  0009512a: str.w r3,[r4,#0x1c8]
  0009512e: str.w r6,[r4,#0x1cc]
  00095132: str.w r12,[r4,#0x1d0]
  00095136: strd r10,r2,[r4,#0x1d4]
  0009513a: strd r1,r12,[r4,#0x1dc]
  0009513e: strd lr,r9,[r4,#0x1e4]
  00095142: strd r11,r12,[r4,#0x1ec]
  00095146: ldr r1,[sp,#0x0]
  00095148: strd r1,r8,[r4,#0x1f4]
  0009514c: strd r5,r0,[r4,#0x1fc]
  00095150: pop.w {r1,r8,r9,r10,r11}
  00095154: pop {r4,r5,r6,r7,pc}
  00095156: movw r0,#0x1702
  0009515a: blx 0x0006ff64
  0009515e: add.w r4,r4,#0x428
  00095162: mov r0,r5
  00095164: mov r1,r4
  00095166: blx 0x00070750
  0009516a: mov r0,r4
  0009516c: blx 0x0006fea4
  00095170: mov.w r0,#0x1700
  00095174: pop.w {r3,r8,r9,r10,r11}
  00095178: pop.w {r4,r5,r6,r7,lr}
  0009517c: b.w 0x001ab638
```
