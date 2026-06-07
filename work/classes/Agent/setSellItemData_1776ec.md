# Agent::setSellItemData
elf 0x1776ec body 6
Sig: undefined __thiscall setSellItemData(Agent * this, int param_1, int param_2, int param_3)

## decompile
```c

/* Agent::setSellItemData(int, int, int) */

Agent * __thiscall Agent::setSellItemData(Agent *this,int param_1,int param_2,int param_3)

{
  *(int *)(this + 0x34) = param_1;
  *(int *)(this + 0x38) = param_2;
  *(int *)(this + 0x3c) = param_3;
  return this + 0x40;
}

```

## target disasm
```
  001876ec: adds r0,#0x34
  001876ee: stmia r0!,{r1,r2,r3}
  001876f0: bx lr
```
