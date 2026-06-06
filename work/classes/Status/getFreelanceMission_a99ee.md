# Status::getFreelanceMission
elf 0xa99ee body 10
Sig: undefined __stdcall getFreelanceMission(void)

## decompile
```c

/* Status::getFreelanceMission() */

undefined4 Status::getFreelanceMission(void)

{
  int in_r0;
  
  return *(undefined4 *)(*(int *)(*(int *)(in_r0 + 0x198) + 4) + 4);
}

```
## target disasm
```
  000b99ee: ldr.w r0,[r0,#0x198]
  000b99f2: ldr r0,[r0,#0x4]
  000b99f4: ldr r0,[r0,#0x4]
  000b99f6: bx lr
```
