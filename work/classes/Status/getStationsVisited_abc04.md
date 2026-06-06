# Status::getStationsVisited
elf 0xabc04 body 6
Sig: undefined __thiscall getStationsVisited(Status * this)

## decompile
```c

/* Status::getStationsVisited() */

undefined4 __thiscall Status::getStationsVisited(Status *this)

{
  return *(undefined4 *)(this + 0x1d0);
}

```
## target disasm
```
  000bbc04: ldr.w r0,[r0,#0x1d0]
  000bbc08: bx lr
```
