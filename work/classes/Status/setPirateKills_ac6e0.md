# Status::setPirateKills
elf 0xac6e0 body 6
Sig: undefined __thiscall setPirateKills(Status * this, int param_1)

## decompile
```c

/* Status::setPirateKills(int) */

void __thiscall Status::setPirateKills(Status *this,int param_1)

{
  *(int *)(this + 0x1d8) = param_1;
  return;
}

```
## target disasm
```
  000bc6e0: str.w r1,[r0,#0x1d8]
  000bc6e4: bx lr
```
