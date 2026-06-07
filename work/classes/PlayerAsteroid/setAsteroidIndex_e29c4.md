# PlayerAsteroid::setAsteroidIndex
elf 0xe29c4 body 6
Sig: undefined __thiscall setAsteroidIndex(PlayerAsteroid * this, int param_1)

## decompile
```c

/* PlayerAsteroid::setAsteroidIndex(int) */

void __thiscall PlayerAsteroid::setAsteroidIndex(PlayerAsteroid *this,int param_1)

{
  *(int *)(this + 0x128) = param_1;
  return;
}

```

## target disasm
```
  000f29c4: str.w r1,[r0,#0x128]
  000f29c8: bx lr
```
