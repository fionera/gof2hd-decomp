# CheatHandler::CheatHandler
elf 0x74448 body 42
Sig: undefined __thiscall CheatHandler(CheatHandler * this, KeyCode * param_1)

## decompile
```c

/* AbyssEngine::CheatHandler::CheatHandler(AbyssEngine::KeyCode*) */

CheatHandler * __thiscall
AbyssEngine::CheatHandler::CheatHandler(CheatHandler *this,KeyCode *param_1)

{
  Array<AbyssEngine::CheatCode*> *this_00;
  
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  this_00 = operator_new(0xc);
  Array<AbyssEngine::CheatCode*>::Array(this_00);
  *(Array<AbyssEngine::CheatCode*> **)(this + 8) = this_00;
  *(KeyCode **)(this + 0xc) = param_1;
  return this;
}

```

## target disasm
```
  00084448: push {r4,r5,r6,r7,lr}
  0008444a: add r7,sp,#0xc
  0008444c: push.w r11
  00084450: mov r4,r0
  00084452: movs r0,#0x0
  00084454: strd r0,r0,[r4,#0x0]
  00084458: movs r0,#0xc
  0008445a: mov r5,r1
  0008445c: blx 0x0006eb24
  00084460: mov r6,r0
  00084462: blx 0x0006f6c4
  00084466: strd r6,r5,[r4,#0x8]
  0008446a: mov r0,r4
  0008446c: pop.w r11
  00084470: pop {r4,r5,r6,r7,pc}
```
