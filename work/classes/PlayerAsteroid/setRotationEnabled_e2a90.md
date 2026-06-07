# PlayerAsteroid::setRotationEnabled
elf 0xe2a90 body 6
Sig: undefined __thiscall setRotationEnabled(PlayerAsteroid * this, bool param_1)

## decompile
```c

/* PlayerAsteroid::setRotationEnabled(bool) */

void __thiscall PlayerAsteroid::setRotationEnabled(PlayerAsteroid *this,bool param_1)

{
  this[0x14c] = (PlayerAsteroid)param_1;
  return;
}

```

## target disasm
```
  000f2a90: strb.w r1,[r0,#0x14c]
  000f2a94: bx lr
```
