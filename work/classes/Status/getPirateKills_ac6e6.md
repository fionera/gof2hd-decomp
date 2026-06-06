# Status::getPirateKills
elf 0xac6e6 body 6
Sig: undefined __thiscall getPirateKills(Status * this)

## decompile
```c

/* Status::getPirateKills() */

undefined4 __thiscall Status::getPirateKills(Status *this)

{
  return *(undefined4 *)(this + 0x1d8);
}

```
## target disasm
```
  000bc6e6: ldr.w r0,[r0,#0x1d8]
  000bc6ea: bx lr
```
