# Mission::isCampaignMission
elf 0x15bb7e body 10
Sig: undefined __thiscall isCampaignMission(Mission * this)

## decompile
```c

/* Mission::isCampaignMission() */

bool __thiscall Mission::isCampaignMission(Mission *this)

{
  return *(int *)(this + 100) != 0;
}

```

## target disasm
```
  0016bb7e: ldr r0,[r0,#0x64]
  0016bb80: cmp r0,#0x0
  0016bb82: it ne
  0016bb84: mov.ne r0,#0x1
  0016bb86: bx lr
```
