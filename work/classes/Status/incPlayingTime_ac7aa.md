# Status::incPlayingTime
elf 0xac7aa body 16
Sig: undefined __stdcall incPlayingTime(longlong param_1)

## decompile
```c

/* Status::incPlayingTime(long long) */

void Status::incPlayingTime(longlong param_1)

{
  int iVar1;
  uint uVar2;
  uint in_r2;
  int in_r3;
  
  iVar1 = (int)param_1;
  uVar2 = *(uint *)(iVar1 + 0x1b8);
  *(uint *)(iVar1 + 0x1b8) = uVar2 + in_r2;
  *(uint *)(iVar1 + 0x1bc) = *(int *)(iVar1 + 0x1bc) + in_r3 + (uint)CARRY4(uVar2,in_r2);
  return;
}

```
## target disasm
```
  000bc7aa: ldrd r1,r12,[r0,#0x1b8]
  000bc7ae: adds r1,r1,r2
  000bc7b0: adc.w r2,r12,r3
  000bc7b4: strd r1,r2,[r0,#0x1b8]
  000bc7b8: bx lr
```
