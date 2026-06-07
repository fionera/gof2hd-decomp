# Objective::setAchievedText
elf 0x980cc body 36
Sig: undefined __thiscall setAchievedText(Objective * this, String * param_1)

## decompile
```c

/* Objective::setAchievedText(AbyssEngine::String*) */

void __thiscall Objective::setAchievedText(Objective *this,String *param_1)

{
  String *this_00;
  
  this_00 = operator_new(0xc);
  AbyssEngine::String::String(this_00,param_1,false);
  *(String **)(this + 0x14) = this_00;
  return;
}

```

## target disasm
```
  000a80cc: push {r4,r5,r6,r7,lr}
  000a80ce: add r7,sp,#0xc
  000a80d0: push.w r11
  000a80d4: mov r4,r0
  000a80d6: movs r0,#0xc
  000a80d8: mov r6,r1
  000a80da: blx 0x0006eb24
  000a80de: mov r5,r0
  000a80e0: mov r1,r6
  000a80e2: movs r2,#0x0
  000a80e4: blx 0x0006f028
  000a80e8: str r5,[r4,#0x14]
  000a80ea: pop.w r11
  000a80ee: pop {r4,r5,r6,r7,pc}
```
