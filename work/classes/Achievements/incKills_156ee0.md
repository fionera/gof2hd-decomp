# Achievements::incKills
elf 0x156ee0 body 8
Sig: undefined __thiscall incKills(Achievements * this)

## decompile
```c

/* Achievements::incKills() */

void __thiscall Achievements::incKills(Achievements *this)

{
  *(int *)(this + 8) = *(int *)(this + 8) + 1;
  return;
}

```

## target disasm
```
  00166ee0: ldr r1,[r0,#0x8]
  00166ee2: adds r1,#0x1
  00166ee4: str r1,[r0,#0x8]
  00166ee6: bx lr
```
