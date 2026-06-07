# Achievements::updateCredits
elf 0x156f0a body 10
Sig: undefined __thiscall updateCredits(Achievements * this, int param_1)

## decompile
```c

/* Achievements::updateCredits(int) */

void __thiscall Achievements::updateCredits(Achievements *this,int param_1)

{
  if (*(int *)(this + 0x1c) < param_1) {
    *(int *)(this + 0x1c) = param_1;
  }
  return;
}

```

## target disasm
```
  00166f0a: ldr r2,[r0,#0x1c]
  00166f0c: cmp r2,r1
  00166f0e: it lt
  00166f10: str.lt r1,[r0,#0x1c]
  00166f12: bx lr
```
