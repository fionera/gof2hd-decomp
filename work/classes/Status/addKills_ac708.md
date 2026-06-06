# Status::addKills
elf 0xac708 body 12
Sig: undefined __thiscall addKills(Status * this, int param_1)

## decompile
```c

/* Status::addKills(int) */

void __thiscall Status::addKills(Status *this,int param_1)

{
  *(int *)(this + 0x1c0) = param_1 + *(int *)(this + 0x1c0);
  return;
}

```
## target disasm
```
  000bc708: ldr.w r2,[r0,#0x1c0]
  000bc70c: add r1,r2
  000bc70e: str.w r1,[r0,#0x1c0]
  000bc712: bx lr
```
