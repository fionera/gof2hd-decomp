# Status::getWanted
elf 0xace4e body 4
Sig: undefined __thiscall getWanted(Status * this)

## decompile
```c

/* Status::getWanted() */

undefined4 __thiscall Status::getWanted(Status *this)

{
  return *(undefined4 *)this;
}

```
## target disasm
```
  000bce4e: ldr r0,[r0,#0x0]
  000bce50: bx lr
```
