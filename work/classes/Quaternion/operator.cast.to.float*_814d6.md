# AbyssEngine::Quaternion::operator.cast.to.float*
elf 0x814d6 body 2
Sig: float * __thiscall operator.cast.to.float*(Quaternion * this)

## decompile
```c

/* AbyssEngine::Quaternion::operator float const*() const */

float * __thiscall AbyssEngine::Quaternion::operator_cast_to_float_(Quaternion *this)

{
  return (float *)this;
}

```
## target disasm
```
  000914d6: bx lr
```
