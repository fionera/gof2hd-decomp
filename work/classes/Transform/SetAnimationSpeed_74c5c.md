# Transform::SetAnimationSpeed
elf 0x74c5c body 6
Sig: undefined __thiscall SetAnimationSpeed(Transform * this, float param_1)

## decompile
```c

/* AbyssEngine::Transform::SetAnimationSpeed(float) */

void __thiscall AbyssEngine::Transform::SetAnimationSpeed(Transform *this,float param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(this + 0xf0) = in_r1;
  return;
}

```

## target disasm
```
  00084c5c: str.w r1,[r0,#0xf0]
  00084c60: bx lr
```
