# AbyssEngine::Quaternion::Quaternion
elf 0x8102c body 14
Sig: undefined __thiscall Quaternion(Quaternion * this, Matrix * param_1)

## decompile
```c

/* AbyssEngine::Quaternion::Quaternion(AbyssEngine::AEMath::Matrix const&) */

Quaternion * __thiscall AbyssEngine::Quaternion::Quaternion(Quaternion *this,Matrix *param_1)

{
  Set(this,param_1);
  return this;
}

```
## target disasm
```
  0009102c: push {r4,r6,r7,lr}
  0009102e: add r7,sp,#0x8
  00091030: mov r4,r0
  00091032: blx 0x0006f154
  00091036: mov r0,r4
  00091038: pop {r4,r6,r7,pc}
```
