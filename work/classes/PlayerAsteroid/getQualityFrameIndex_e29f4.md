# PlayerAsteroid::getQualityFrameIndex
elf 0xe29f4 body 10
Sig: undefined __thiscall getQualityFrameIndex(PlayerAsteroid * this)

## decompile
```c

/* PlayerAsteroid::getQualityFrameIndex() */

int __thiscall PlayerAsteroid::getQualityFrameIndex(PlayerAsteroid *this)

{
  return 7 - *(int *)(this + 0x150);
}

```

## target disasm
```
  000f29f4: ldr.w r0,[r0,#0x150]
  000f29f8: rsb.w r0,r0,#0x7
  000f29fc: bx lr
```
