# Globals::drawLines
elf 0xe4688 body 20
Sig: undefined __thiscall drawLines(Globals * this, uint param_1, Array * param_2, int param_3, int param_4)

## decompile
```c

/* Globals::drawLines(unsigned int, Array<AbyssEngine::String*>*, int, int) */

void __thiscall
Globals::drawLines(Globals *this,uint param_1,Array *param_2,int param_3,int param_4)

{
  drawLines(0,(Array *)param_1,(int)param_2,param_3,SUB41(param_4,0));
  return;
}

```

## target disasm
```
  000f4688: push {r5,r6,r7,lr}
  000f468a: add r7,sp,#0x8
  000f468c: movs r0,#0x0
  000f468e: ldr.w r12,[r7,#0x8]
  000f4692: strd r12,r0,[sp,#0x0]
  000f4696: blx 0x00074e9c
  000f469a: pop {r2,r3,r7,pc}
```
