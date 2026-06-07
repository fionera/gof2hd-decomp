# Achievements::resetPirateKills
elf 0x156ef8 body 6
Sig: undefined __thiscall resetPirateKills(Achievements * this)

## decompile
```c

/* Achievements::resetPirateKills() */

void __thiscall Achievements::resetPirateKills(Achievements *this)

{
  *(undefined4 *)(this + 0x10) = 0;
  return;
}

```

## target disasm
```
  00166ef8: movs r1,#0x0
  00166efa: str r1,[r0,#0x10]
  00166efc: bx lr
```
