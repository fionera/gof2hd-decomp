# Status::setMission
elf 0xa9652 body 6
Sig: undefined __stdcall setMission(Mission * param_1)

## decompile
```c

/* Status::setMission(Mission*) */

void Status::setMission(Mission *param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 0x194) = in_r1;
  return;
}

```
## target disasm
```
  000b9652: str.w r1,[r0,#0x194]
  000b9656: bx lr
```
