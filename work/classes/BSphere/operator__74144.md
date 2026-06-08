# BSphere::operator=
elf 0x74144 body 16
Sig: undefined __thiscall operator=(BSphere * this, BSphere * param_1)

## decompile
```c

/* AbyssEngine::AEMath::BSphere::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::BSphere const&) */

void __thiscall AbyssEngine::AEMath::BSphere::operator=(BSphere *this,BSphere *param_1)

{
  undefined8 uVar1;
  
  uVar1 = *(undefined8 *)(param_1 + 8);
  *(undefined8 *)this = *(undefined8 *)param_1;
  *(undefined8 *)(this + 8) = uVar1;
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(param_1 + 0x10);
  return;
}

```

## target disasm
```
  00084144: vld1.32 {d16,d17},[r1]!
  00084148: mov r2,r0
  0008414a: vst1.32 {d16,d17},[r2]!
  0008414e: ldr r1,[r1,#0x0]
  00084150: str r1,[r2,#0x0]
  00084152: bx lr
```
