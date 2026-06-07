# PlayerEgo::resetChargingDrive
elf 0x9b04e body 8
Sig: undefined __thiscall resetChargingDrive(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::resetChargingDrive() */

void __thiscall PlayerEgo::resetChargingDrive(PlayerEgo *this)

{
  *(undefined1 *)&this[0xe].m_pLevel = 0;
  return;
}

```

## target disasm
```
  000ab04e: movs r1,#0x0
  000ab050: strb.w r1,[r0,#0x204]
  000ab054: bx lr
```
