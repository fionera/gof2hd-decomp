# Layout::drawBGBorder
elf 0xd393c body 32
Sig: undefined __stdcall drawBGBorder(uint param_1, uint param_2, int param_3, int param_4, int param_5, int param_6)

## decompile
```c

/* Layout::drawBGBorder(unsigned int, unsigned int, int, int, int, int) */

void Layout::drawBGBorder(uint param_1,uint param_2,int param_3,int param_4,int param_5,int param_6)

{
  int in_stack_00000008;
  
  drawBGBorder((Layout *)param_5,param_2,param_3,param_4,param_5,param_6,in_stack_00000008,0,0);
  return;
}

```

## target disasm
```
  000e393c: push {r4,r6,r7,lr}
  000e393e: add r7,sp,#0x8
  000e3940: sub sp,#0x18
  000e3942: add.w lr,r7,#0x8
  000e3946: movs r4,#0x0
  000e3948: ldm.w lr,{r0,r12,lr}
  000e394c: stm sp,{r0,r12,lr}
  000e3950: strd r4,r4,[sp,#0xc]
  000e3954: blx 0x00074e30
  000e3958: add sp,#0x18
  000e395a: pop {r4,r6,r7,pc}
```
