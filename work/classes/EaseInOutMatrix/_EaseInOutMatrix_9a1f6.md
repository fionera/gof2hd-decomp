# EaseInOutMatrix::~EaseInOutMatrix
elf 0x9a1f6 body 24
Sig: undefined __thiscall ~EaseInOutMatrix(EaseInOutMatrix * this)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix() */

EaseInOutMatrix * __thiscall AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix(EaseInOutMatrix *this)

{
  Quaternion::~Quaternion((Quaternion *)(this + 0x58));
  Quaternion::~Quaternion((Quaternion *)(this + 0x3c));
  return this;
}

```

## target disasm
```
  000aa1f6: push {r4,r6,r7,lr}
  000aa1f8: add r7,sp,#0x8
  000aa1fa: mov r4,r0
  000aa1fc: adds r0,#0x58
  000aa1fe: blx 0x0006f13c
  000aa202: add.w r0,r4,#0x3c
  000aa206: blx 0x0006f13c
  000aa20a: mov r0,r4
  000aa20c: pop {r4,r6,r7,pc}
```
