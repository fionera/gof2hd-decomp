# AbyssEngine::Quaternion::Quaternion
elf 0x81018 body 14
Sig: undefined __thiscall Quaternion(Quaternion * this, Vector param_1)

## decompile
```c

/* AbyssEngine::Quaternion::Quaternion(AbyssEngine::AEMath::Vector) */

Quaternion * __thiscall AbyssEngine::Quaternion::Quaternion(Quaternion *this)

{
  Set();
  return this;
}

```
## target disasm
```
  00091018: push {r4,r6,r7,lr}
  0009101a: add r7,sp,#0x8
  0009101c: mov r4,r0
  0009101e: blx 0x000701c8
  00091022: mov r0,r4
  00091024: pop {r4,r6,r7,pc}
```
