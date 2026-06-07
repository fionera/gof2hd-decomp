# PlayerAsteroid::~PlayerAsteroid
elf 0xe29b4 body 16
Sig: undefined __thiscall ~PlayerAsteroid(PlayerAsteroid * this)

## decompile
```c

/* PlayerAsteroid::~PlayerAsteroid() */

void __thiscall PlayerAsteroid::~PlayerAsteroid(PlayerAsteroid *this)

{
  ~PlayerAsteroid(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000f29b4: push {r7,lr}
  000f29b6: mov r7,sp
  000f29b8: blx 0x00075bec
  000f29bc: pop.w {r7,lr}
  000f29c0: b.w 0x001ab098
```
