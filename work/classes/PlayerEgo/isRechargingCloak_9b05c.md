# PlayerEgo::isRechargingCloak
elf 0x9b05c body 14
Sig: undefined __thiscall isRechargingCloak(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::isRechargingCloak() */

bool __thiscall PlayerEgo::isRechargingCloak(PlayerEgo *this)

{
  return 0 < (int)this[0xe].field_14;
}

```

## target disasm
```
  000ab05c: ldr.w r1,[r0,#0x20c]
  000ab060: movs r0,#0x0
  000ab062: cmp r1,#0x0
  000ab064: it gt
  000ab066: mov.gt r0,#0x1
  000ab068: bx lr
```
