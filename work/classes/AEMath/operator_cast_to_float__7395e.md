# AbyssEngine::AEMath::Vector::operator.cast.to.float*
elf 0x7395e body 2
Sig: float * __thiscall operator.cast.to.float*(Vector * this)

## decompile
```c

/* AbyssEngine::AEMath::Vector::operator float const*() const */

float * __thiscall AbyssEngine::AEMath::Vector::operator_cast_to_float_(Vector *this)

{
  return (float *)this;
}

```
## target disasm
```
  0008395e: bx lr
```
