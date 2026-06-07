# GameText::setSubstituteArray
elf 0x817a0 body 48
Sig: undefined __thiscall setSubstituteArray(GameText * this, int * param_1, uint param_2)

## decompile
```c

/* GameText::setSubstituteArray(int*, unsigned int) */

void __thiscall GameText::setSubstituteArray(GameText *this,int *param_1,uint param_2)

{
  int iVar1;
  
  if (param_2 != 0) {
    if ((param_2 & 1) != 0) {
      return;
    }
    ArrayRemoveAll<int>((Array *)this);
  }
  for (; param_2 != 0; param_2 = param_2 - 1) {
    iVar1 = *param_1;
    param_1 = param_1 + 1;
    ArrayAdd<int>(iVar1,(Array *)this);
  }
  return;
}

```

## target disasm
```
  000917a0: push {r4,r5,r6,r7,lr}
  000917a2: add r7,sp,#0xc
  000917a4: push.w r11
  000917a8: mov r4,r2
  000917aa: mov r5,r1
  000917ac: mov r6,r0
  000917ae: cbz r2,0x000917c6
  000917b0: lsls r0,r4,#0x1f
  000917b2: bne 0x000917ca
  000917b4: mov r0,r6
  000917b6: blx 0x00070210
  000917ba: b 0x000917c6
  000917bc: ldmia r5!,{r0}
  000917be: mov r1,r6
  000917c0: blx 0x0007021c
  000917c4: subs r4,#0x1
  000917c6: cmp r4,#0x0
  000917c8: bne 0x000917bc
  000917ca: pop.w r11
  000917ce: pop {r4,r5,r6,r7,pc}
```
