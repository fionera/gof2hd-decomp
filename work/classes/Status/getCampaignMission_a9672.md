# Status::getCampaignMission
elf 0xa9672 body 10
Sig: undefined __stdcall getCampaignMission(void)

## decompile
```c

/* Status::getCampaignMission() */

undefined4 Status::getCampaignMission(void)

{
  int in_r0;
  
  return **(undefined4 **)(*(int *)(in_r0 + 0x198) + 4);
}

```
## target disasm
```
  000b9672: ldr.w r0,[r0,#0x198]
  000b9676: ldr r0,[r0,#0x4]
  000b9678: ldr r0,[r0,#0x0]
  000b967a: bx lr
```
