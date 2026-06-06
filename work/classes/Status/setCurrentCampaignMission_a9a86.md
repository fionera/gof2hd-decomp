# Status::setCurrentCampaignMission
elf 0xa9a86 body 6
Sig: undefined __stdcall setCurrentCampaignMission(int param_1)

## decompile
```c

/* Status::setCurrentCampaignMission(int) */

void Status::setCurrentCampaignMission(int param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 0x1e8) = in_r1;
  return;
}

```
## target disasm
```
  000b9a86: str.w r1,[r0,#0x1e8]
  000b9a8a: bx lr
```
