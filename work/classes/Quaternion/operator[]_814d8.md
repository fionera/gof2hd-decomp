# AbyssEngine::Quaternion::operator[]
elf 0x814d8 body 6
Sig: undefined __thiscall operator[](Quaternion * this, int param_1)

## decompile
```c

/* AbyssEngine::Quaternion::operator[](int) */

Quaternion * __thiscall AbyssEngine::Quaternion::operator[](Quaternion *this,int param_1)

{
  return this + param_1 * 4;
}

```
## target disasm
```
  000914d8: add.w r0,r0,r1, lsl #0x2
  000914dc: bx lr
```
