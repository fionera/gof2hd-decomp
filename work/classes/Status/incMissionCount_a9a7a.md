# Status::incMissionCount
elf 0xa9a7a body 12
Sig: undefined __thiscall incMissionCount(Status * this)

## decompile
```c

/* Status::incMissionCount() */

void __thiscall Status::incMissionCount(Status *this)

{
  *(int *)(this + 0x1c4) = *(int *)(this + 0x1c4) + 1;
  return;
}

```
## target disasm
```
  000b9a7a: ldr.w r1,[r0,#0x1c4]
  000b9a7e: adds r1,#0x1
  000b9a80: str.w r1,[r0,#0x1c4]
  000b9a84: bx lr
```
