# PlayerEgo::hasCloak
elf 0x9a9b0 body 12
Sig: undefined __thiscall hasCloak(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::hasCloak() */

bool __thiscall PlayerEgo::hasCloak(PlayerEgo *this)

{
  return this[0xc].m_pPlayer != 0;
}

```

## target disasm
```
  000aa9b0: ldr.w r0,[r0,#0x1b0]
  000aa9b4: cmp r0,#0x0
  000aa9b6: it ne
  000aa9b8: mov.ne r0,#0x1
  000aa9ba: bx lr
```
