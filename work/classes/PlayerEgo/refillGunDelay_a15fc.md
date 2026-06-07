# PlayerEgo::refillGunDelay
elf 0xa15fc body 10
Sig: undefined __thiscall refillGunDelay(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::refillGunDelay() */

void __thiscall PlayerEgo::refillGunDelay(PlayerEgo *this)

{
  (*(code *)(DAT_001abc14 + 0x1abc18))(this->m_pPlayer,0);
  return;
}

```

## target disasm
```
  000b15fc: ldr r0,[r0,#0x0]
  000b15fe: movs r1,#0x0
  000b1600: b.w 0x001abc08
  001abc08: bx pc
```
