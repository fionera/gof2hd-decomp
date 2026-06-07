# Achievements::incPirateKills
elf 0x156eec body 8
Sig: undefined __thiscall incPirateKills(Achievements * this)

## decompile
```c

/* Achievements::incPirateKills() */

void __thiscall Achievements::incPirateKills(Achievements *this)

{
  *(int *)(this + 0x10) = *(int *)(this + 0x10) + 1;
  return;
}

```

## target disasm
```
  00166eec: ldr r1,[r0,#0x10]
  00166eee: adds r1,#0x1
  00166ef0: str r1,[r0,#0x10]
  00166ef2: bx lr
```
