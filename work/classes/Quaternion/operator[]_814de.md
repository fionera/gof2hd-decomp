# AbyssEngine::Quaternion::operator[]
elf 0x814de body 6
Sig: undefined __thiscall operator[](Quaternion * this, int param_1)

## decompile
```c

/* AbyssEngine::Quaternion::operator[](int) const */

undefined4 __thiscall AbyssEngine::Quaternion::operator[](Quaternion *this,int param_1)

{
  return *(undefined4 *)(this + param_1 * 4);
}

```
## target disasm
```
  000914de: ldr.w r0,[r0,r1,lsl #0x2]
  000914e2: bx lr
```
