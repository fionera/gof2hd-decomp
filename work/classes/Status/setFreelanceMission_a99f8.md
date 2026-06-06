# Status::setFreelanceMission
elf 0xa99f8 body 10
Sig: undefined __stdcall setFreelanceMission(Mission * param_1)

## decompile
```c

/* Status::setFreelanceMission(Mission*) */

void Status::setFreelanceMission(Mission *param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x198) + 4) + 4) = in_r1;
  return;
}

```
## target disasm
```
  000b99f8: ldr.w r0,[r0,#0x198]
  000b99fc: ldr r0,[r0,#0x4]
  000b99fe: str r1,[r0,#0x4]
  000b9a00: bx lr
```
