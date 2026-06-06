# AbyssEngine::AEMath::Matrix::operator=
elf 0x80130 body 58
Sig: undefined __thiscall operator=(Matrix * this, Matrix * param_1)

## decompile
```c

/* AbyssEngine::AEMath::Matrix::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Matrix const&) */

Matrix * __thiscall AbyssEngine::AEMath::Matrix::operator=(Matrix *this,Matrix *param_1)

{
  undefined8 uVar1;
  
  uVar1 = *(undefined8 *)(param_1 + 8);
  *(undefined8 *)this = *(undefined8 *)param_1;
  *(undefined8 *)(this + 8) = uVar1;
  uVar1 = *(undefined8 *)(param_1 + 0x18);
  *(undefined8 *)(this + 0x10) = *(undefined8 *)(param_1 + 0x10);
  *(undefined8 *)(this + 0x18) = uVar1;
  uVar1 = *(undefined8 *)(param_1 + 0x28);
  *(undefined8 *)(this + 0x20) = *(undefined8 *)(param_1 + 0x20);
  *(undefined8 *)(this + 0x28) = uVar1;
  Vector::operator=((Vector *)(this + 0x30),(Vector *)(param_1 + 0x30));
  return this;
}

```
## target disasm
```
  00090130: push {r4,r6,r7,lr}
  00090132: add r7,sp,#0x8
  00090134: movs r3,#0x30
  00090136: mov r2,r1
  00090138: vld1.32 {d16,d17},[r2],r3
  0009013c: mov r4,r0
  0009013e: vst1.32 {d16,d17},[r0],r3
  00090142: add.w r3,r1,#0x10
  00090146: adds r1,#0x20
  00090148: vld1.32 {d16,d17},[r3]
  0009014c: add.w r3,r4,#0x10
  00090150: vst1.32 {d16,d17},[r3]
  00090154: vld1.32 {d16,d17},[r1]
  00090158: add.w r1,r4,#0x20
  0009015c: vst1.32 {d16,d17},[r1]
  00090160: mov r1,r2
  00090162: blx 0x0006eb3c
  00090166: mov r0,r4
  00090168: pop {r4,r6,r7,pc}
```
