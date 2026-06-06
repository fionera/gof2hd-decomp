# Status::getCurrentCampaignMission
elf 0xa966c body 6
Sig: undefined __stdcall getCurrentCampaignMission(void)

## decompile
```c

/* Status::getCurrentCampaignMission() */

undefined4 Status::getCurrentCampaignMission(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x1e8);
}

```
## target disasm
```
  000b966c: ldr.w r0,[r0,#0x1e8]
  000b9670: bx lr
```
