# Ship::getFreeSlots
elf 0x17541e body 18
Sig: undefined __thiscall getFreeSlots(Ship * this, int param_1)

## decompile
```c

/* Ship::getFreeSlots(int) */

int __thiscall Ship::getFreeSlots(Ship *this,int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(*(int *)(this + 0x68) + param_1 * 4);
  iVar1 = getUsedSlots(this,param_1);
  return iVar2 - iVar1;
}

```
## target disasm
```
  0018541e: push {r4,r6,r7,lr}
  00185420: add r7,sp,#0x8
  00185422: ldr r2,[r0,#0x68]
  00185424: ldr.w r4,[r2,r1,lsl #0x2]
  00185428: blx 0x00077e9c
  0018542c: subs r0,r4,r0
  0018542e: pop {r4,r6,r7,pc}
```
