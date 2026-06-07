# PlayerEgo::isChargingCloak
elf 0x9b056 body 6
Sig: undefined __thiscall isChargingCloak(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::isChargingCloak() */

undefined1 __thiscall PlayerEgo::isChargingCloak(PlayerEgo *this)

{
  return *(undefined1 *)((int)&this[0xb].field_20 + 1);
}

```

## target disasm
```
  000ab056: ldrb.w r0,[r0,#0x1ad]
  000ab05a: bx lr
```
