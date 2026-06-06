# Status::getAgents
elf 0xac7a6 body 4
Sig: undefined __thiscall getAgents(Status * this)

## decompile
```c

/* Status::getAgents() */

undefined4 __thiscall Status::getAgents(Status *this)

{
  return *(undefined4 *)(this + 0x20);
}

```
## target disasm
```
  000bc7a6: ldr r0,[r0,#0x20]
  000bc7a8: bx lr
```
