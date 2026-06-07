# PlayerEgo::forceBoost
elf 0x9af06 body 30
Sig: undefined __thiscall forceBoost(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::forceBoost() */

void __thiscall PlayerEgo::forceBoost(PlayerEgo *this)

{
  *(undefined1 *)&this[8].field_1C = 1;
  this[8].field_18 = (int *)0x0;
  this[5].field_4 = (int *)0x41000000;
  this[5].field_18 = (int *)0x2710;
  this[5].field_1C = 0;
  return;
}

```

## target disasm
```
  000aaf06: movs r1,#0x1
  000aaf08: mov.w r2,#0x41000000
  000aaf0c: strb.w r1,[r0,#0x13c]
  000aaf10: movs r1,#0x0
  000aaf12: str.w r1,[r0,#0x138]
  000aaf16: str.w r2,[r0,#0xb8]
  000aaf1a: movw r2,#0x2710
  000aaf1e: strd r2,r1,[r0,#0xcc]
  000aaf22: bx lr
```
