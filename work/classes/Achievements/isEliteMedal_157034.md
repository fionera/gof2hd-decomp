# Achievements::isEliteMedal
elf 0x157034 body 10
Sig: undefined __thiscall isEliteMedal(Achievements * this, int param_1)

## decompile
```c

/* Achievements::isEliteMedal(int) */

bool __thiscall Achievements::isEliteMedal(Achievements *this,int param_1)

{
  return 0x23 < param_1;
}

```

## target disasm
```
  00167034: movs r0,#0x0
  00167036: cmp r1,#0x23
  00167038: it gt
  0016703a: mov.gt r0,#0x1
  0016703c: bx lr
```
