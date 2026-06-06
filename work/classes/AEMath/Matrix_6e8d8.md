# AbyssEngine::AEMath::Matrix::Matrix
elf 0x6e8d8 body 44
Sig: undefined __thiscall Matrix(Matrix * this)

## decompile
```c

/* AbyssEngine::AEMath::Matrix::Matrix() */

void __thiscall AbyssEngine::AEMath::Matrix::Matrix(Matrix *this)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar2 = DAT_0007e910;
  uVar1 = DAT_0007e908;
  *(undefined4 *)this = 0x3f800000;
  *(undefined4 *)(this + 0x14) = 0x3f800000;
  *(undefined4 *)(this + 0x38) = 0x3f800000;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x20) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x24) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined8 *)(this + 0x28) = uVar1;
  *(undefined8 *)(this + 0x30) = uVar2;
  return;
}

```
## target disasm
```
  0007e8d8: adr r1,[0x7e908]
  0007e8da: vmov.i32 q9,#0x0
  0007e8de: vld1.64 {d16,d17},[r1]
  0007e8e2: mov.w r1,#0x3f800000
  0007e8e6: str r1,[r0,#0x0]
  0007e8e8: str r1,[r0,#0x14]
  0007e8ea: str r1,[r0,#0x38]
  0007e8ec: adds r1,r0,#0x4
  0007e8ee: vst1.32 {d18,d19},[r1]
  0007e8f2: add.w r1,r0,#0x18
  0007e8f6: vst1.32 {d18,d19},[r1]
  0007e8fa: add.w r1,r0,#0x28
  0007e8fe: vst1.32 {d16,d17},[r1]
  0007e902: bx lr
```
