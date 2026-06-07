# EaseInOutMatrix::EaseInOutMatrix
elf 0x6e820 body 154
Sig: undefined __thiscall EaseInOutMatrix(EaseInOutMatrix * this)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::EaseInOutMatrix() */

EaseInOutMatrix * __thiscall AbyssEngine::EaseInOutMatrix::EaseInOutMatrix(EaseInOutMatrix *this)

{
  int iVar1;
  
  iVar1 = AEMath::Matrix::Matrix((Matrix *)this);
  Quaternion::Quaternion((Quaternion *)(iVar1 + 0x3c));
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  Quaternion::Quaternion((Quaternion *)(this + 0x58));
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  AEMath::Matrix::Matrix((Matrix *)(this + 0x78));
  AEMath::Matrix::Matrix((Matrix *)(this + 0xb4));
  SetRange(this,0x3f800000,0,0,0,0,0x3f800000,0,0,0,0,0x3f800000,0,0x3f800000,0x3f800000,0x3f800000,
           0x3f800000,0,0,0,0,0x3f800000,0,0,0,0,0x3f800000,0,0x3f800000,0x3f800000,0x3f800000);
  *(undefined4 *)(this + 0xf0) = 0;
  return this;
}

```

## target disasm
```
  0007e820: push {r4,r5,r6,r7,lr}
  0007e822: add r7,sp,#0xc
  0007e824: push.w r8
  0007e828: sub sp,#0x70
  0007e82a: mov r5,r0
  0007e82c: blx 0x0006f118
  0007e830: add.w r8,r0,#0x3c
  0007e834: mov r0,r8
  0007e836: blx 0x0006f124
  0007e83a: add.w r6,r5,#0x58
  0007e83e: movs r4,#0x0
  0007e840: strd r4,r4,[r5,#0x4c]
  0007e844: str r4,[r5,#0x54]
  0007e846: mov r0,r6
  0007e848: blx 0x0006f124
  0007e84c: strd r4,r4,[r5,#0x68]
  0007e850: str r4,[r5,#0x70]
  0007e852: add.w r0,r5,#0x78
  0007e856: blx 0x0006f118
  0007e85a: add.w r0,r5,#0xb4
  0007e85e: blx 0x0006f118
  0007e862: mov.w r0,#0x3f800000
  0007e866: movs r4,#0x0
  0007e868: strd r0,r0,[sp,#0x60]
  0007e86c: mov.w r1,#0x3f800000
  0007e870: str r0,[sp,#0x68]
  0007e872: movs r2,#0x0
  0007e874: strd r4,r0,[sp,#0x40]
  0007e878: movs r3,#0x0
  0007e87a: strd r4,r4,[sp,#0x48]
  0007e87e: strd r4,r4,[sp,#0x50]
  0007e882: strd r0,r4,[sp,#0x58]
  0007e886: strd r4,r0,[sp,#0x20]
  0007e88a: strd r0,r0,[sp,#0x28]
  0007e88e: strd r0,r4,[sp,#0x30]
  0007e892: strd r4,r4,[sp,#0x38]
  0007e896: strd r4,r4,[sp,#0x0]
  0007e89a: strd r0,r4,[sp,#0x8]
  0007e89e: strd r4,r4,[sp,#0x10]
  0007e8a2: strd r4,r0,[sp,#0x18]
  0007e8a6: mov r0,r5
  0007e8a8: blx 0x0006f130
  0007e8ac: str.w r4,[r5,#0xf0]
  0007e8b0: mov r0,r5
  0007e8b2: add sp,#0x70
  0007e8b4: pop.w r8
  0007e8b8: pop {r4,r5,r6,r7,pc}
```
