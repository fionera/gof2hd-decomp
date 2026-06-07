# PlayerEgo::readyToBoost
elf 0x9af38 body 16
Sig: undefined __thiscall readyToBoost(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::readyToBoost() */

bool __thiscall PlayerEgo::readyToBoost(PlayerEgo *this)

{
  return this[8].field_18 < (int *)0x80000000;
}

```

## target disasm
```
  000aaf38: ldr.w r1,[r0,#0x138]
  000aaf3c: movs r0,#0x0
  000aaf3e: cmp.w r1,#0xffffffff
  000aaf42: it gt
  000aaf44: mov.gt r0,#0x1
  000aaf46: bx lr
```
