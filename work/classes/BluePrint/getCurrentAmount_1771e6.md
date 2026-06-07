# BluePrint::getCurrentAmount
elf 0x1771e6 body 34
Sig: undefined __thiscall getCurrentAmount(BluePrint * this, int param_1)

## decompile
```c

/* BluePrint::getCurrentAmount(int) */

int __thiscall BluePrint::getCurrentAmount(BluePrint *this,int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = getTotalAmount(this,param_1);
  iVar2 = getRemainingAmount(this,param_1);
  return iVar1 - iVar2;
}

```

## target disasm
```
  001871e6: push {r4,r5,r6,r7,lr}
  001871e8: add r7,sp,#0xc
  001871ea: push.w r11
  001871ee: mov r4,r1
  001871f0: mov r5,r0
  001871f2: blx 0x00077044
  001871f6: mov r6,r0
  001871f8: mov r0,r5
  001871fa: mov r1,r4
  001871fc: blx 0x00076f84
  00187200: subs r0,r6,r0
  00187202: pop.w r11
  00187206: pop {r4,r5,r6,r7,pc}
```
