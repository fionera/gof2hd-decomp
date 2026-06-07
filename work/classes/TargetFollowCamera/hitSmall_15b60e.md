# TargetFollowCamera::hitSmall
elf 0x15b60e body 30
Sig: undefined __thiscall hitSmall(TargetFollowCamera * this)

## decompile
```c

/* TargetFollowCamera::hitSmall() */

void __thiscall TargetFollowCamera::hitSmall(TargetFollowCamera *this)

{
  if (*(char *)((int)&this->field_44 + 3) != '\0') {
    return;
  }
  this->field_48 = 0x32;
  *(undefined1 *)((int)&this->field_44 + 3) = 1;
  this[1].m_nPosX = 2.8026e-45;
  *(undefined1 *)&this[1].m_nPosY = 1;
  return;
}

```

## target disasm
```
  0016b60e: ldrb.w r1,[r0,#0x47]
  0016b612: cbz r1,0x0016b616
  0016b614: bx lr
  0016b616: movs r1,#0x32
  0016b618: movs r2,#0x2
  0016b61a: str r1,[r0,#0x48]
  0016b61c: movs r1,#0x1
  0016b61e: strb.w r1,[r0,#0x47]
  0016b622: str.w r2,[r0,#0x120]
  0016b626: strb.w r1,[r0,#0x124]
  0016b62a: bx lr
```
