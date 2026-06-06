# AbyssEngine::AEMath::Vector::operator.cast.to.float*
elf 0x7395c body 2
Sig: float * __thiscall operator.cast.to.float*(Vector * this)

## decompile
```c

/* AbyssEngine::AEMath::Vector::operator float*() */

float * __thiscall AbyssEngine::AEMath::Vector::operator_cast_to_float_(Vector *this)

{
  return (float *)this;
}

```
## target disasm
```
  0008395c: bx lr
```
