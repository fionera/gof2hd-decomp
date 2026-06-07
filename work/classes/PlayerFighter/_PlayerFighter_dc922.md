# PlayerFighter::~PlayerFighter
elf 0xdc922 body 16
Sig: undefined __thiscall ~PlayerFighter(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::~PlayerFighter() */

void __thiscall PlayerFighter::~PlayerFighter(PlayerFighter *this)

{
  ~PlayerFighter(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000ec922: push {r7,lr}
  000ec924: mov r7,sp
  000ec926: blx 0x000757cc
  000ec92a: pop.w {r7,lr}
  000ec92e: b.w 0x001ab098
```
