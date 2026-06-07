# PlayerStaticFar::~PlayerStaticFar
elf 0x11c2c4 body 14
Sig: undefined __thiscall ~PlayerStaticFar(PlayerStaticFar * this)

## decompile
```c

/* PlayerStaticFar::~PlayerStaticFar() */

void __thiscall PlayerStaticFar::~PlayerStaticFar(PlayerStaticFar *this)

{
  (*(code *)(DAT_001ac374 + 0x1ac378))(this);
  return;
}

```

## target disasm
```
  0012c2c4: b.w 0x001ac368
  001ac368: bx pc
  001ac36c: ldr r12,[0x1ac374]
  001ac370: add pc,r12,pc
```
