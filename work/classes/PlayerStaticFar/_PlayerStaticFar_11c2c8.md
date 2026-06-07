# PlayerStaticFar::~PlayerStaticFar
elf 0x11c2c8 body 16
Sig: undefined __thiscall ~PlayerStaticFar(PlayerStaticFar * this)

## decompile
```c

/* PlayerStaticFar::~PlayerStaticFar() */

void __thiscall PlayerStaticFar::~PlayerStaticFar(PlayerStaticFar *this)

{
  PlayerStatic::~PlayerStatic((PlayerStatic *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0012c2c8: push {r7,lr}
  0012c2ca: mov r7,sp
  0012c2cc: blx 0x00075a54
  0012c2d0: pop.w {r7,lr}
  0012c2d4: b.w 0x001ab098
```
