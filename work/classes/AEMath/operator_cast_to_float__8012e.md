# AbyssEngine::AEMath::Matrix::operator.cast.to.float*
elf 0x8012e body 2
Sig: float * __thiscall operator.cast.to.float*(Matrix * this)

## decompile
```c

/* AbyssEngine::AEMath::Matrix::operator float const*() const */

float * __thiscall AbyssEngine::AEMath::Matrix::operator_cast_to_float_(Matrix *this)

{
  return (float *)this;
}

```
## target disasm
```
  0009012e: bx lr
```
