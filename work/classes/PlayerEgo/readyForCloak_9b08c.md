# PlayerEgo::readyForCloak
elf 0x9b08c body 30
Sig: undefined __thiscall readyForCloak(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::readyForCloak() */

bool __thiscall PlayerEgo::readyForCloak(PlayerEgo *this)

{
  if (*(char *)((int)&this[0xb].field_20 + 1) == '\0') {
    return false;
  }
  return this[0xe].field_1C <= (int)this[0xe].field_10;
}

```

## target disasm
```
  000ab08c: ldrb.w r2,[r0,#0x1ad]
  000ab090: mov r1,r0
  000ab092: movs r0,#0x0
  000ab094: cmp r2,#0x0
  000ab096: it eq
  000ab098: bx.eq lr
  000ab09a: ldr.w r2,[r1,#0x208]
  000ab09e: ldr.w r1,[r1,#0x214]
  000ab0a2: cmp r2,r1
  000ab0a4: it ge
  000ab0a6: mov.ge r0,#0x1
  000ab0a8: bx lr
```
