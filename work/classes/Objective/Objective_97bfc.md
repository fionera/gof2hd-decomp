# Objective::Objective
elf 0x97bfc body 18
Sig: undefined __thiscall Objective(Objective * this, int param_1, int param_2, Level * param_3)

## decompile
```c

/* Objective::Objective(int, int, Level*) */

void __thiscall Objective::Objective(Objective *this,int param_1,int param_2,Level *param_3)

{
  *(int *)this = param_1;
  *(int *)(this + 4) = param_2;
  *(undefined4 *)(this + 8) = 0;
  *(Level **)(this + 0xc) = param_3;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  return;
}

```

## target disasm
```
  000a7bfc: mov.w r12,#0x0
  000a7c00: stm r0,{r1,r2,r12}
  000a7c04: strd r3,r12,[r0,#0xc]
  000a7c08: strd r12,r12,[r0,#0x14]
  000a7c0c: bx lr
```
