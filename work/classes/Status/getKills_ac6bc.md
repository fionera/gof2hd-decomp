# Status::getKills
elf 0xac6bc body 6
Sig: undefined __thiscall getKills(Status * this)

## decompile
```c

/* Status::getKills() */

undefined4 __thiscall Status::getKills(Status *this)

{
  return *(undefined4 *)(this + 0x1c0);
}

```
## target disasm
```
  000bc6bc: ldr.w r0,[r0,#0x1c0]
  000bc6c0: bx lr
```
