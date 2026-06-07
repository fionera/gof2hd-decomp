# TargetFollowCamera::hit
elf 0x15b5ec body 34
Sig: undefined __thiscall hit(TargetFollowCamera * this)

## decompile
```c

/* TargetFollowCamera::hit() */

void __thiscall TargetFollowCamera::hit(TargetFollowCamera *this)

{
  if (*(char *)((int)&this->field_44 + 3) != '\0') {
    return;
  }
  this->field_48 = 1000;
  *(undefined1 *)((int)&this->field_44 + 3) = 1;
  this[1].m_nPosX = 8.40779e-45;
  *(undefined1 *)&this[1].m_nPosY = 0;
  return;
}

```

## target disasm
```
  0016b5ec: ldrb.w r1,[r0,#0x47]
  0016b5f0: cbz r1,0x0016b5f4
  0016b5f2: bx lr
  0016b5f4: mov.w r1,#0x3e8
  0016b5f8: str r1,[r0,#0x48]
  0016b5fa: movs r1,#0x1
  0016b5fc: strb.w r1,[r0,#0x47]
  0016b600: movs r1,#0x6
  0016b602: str.w r1,[r0,#0x120]
  0016b606: movs r1,#0x0
  0016b608: strb.w r1,[r0,#0x124]
  0016b60c: bx lr
```
