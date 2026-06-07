# StarMap::askForJumpIntoAlienWorld
elf 0xc9b50 body 54
Sig: undefined __thiscall askForJumpIntoAlienWorld(StarMap * this)

## decompile
```c

/* StarMap::askForJumpIntoAlienWorld() */

void __thiscall StarMap::askForJumpIntoAlienWorld(StarMap *this)

{
  undefined4 *puVar1;
  String *pSVar2;
  ChoiceWindow *this_00;
  
  this_00 = *(ChoiceWindow **)(this + 0x5c);
  puVar1 = (undefined4 *)(DAT_000d9b88 + 0xd9b64);
  this[0x120] = (StarMap)0x1;
  pSVar2 = (String *)GameText::getText(*(int *)*puVar1);
  ChoiceWindow::set(this_00,pSVar2,true);
  this[0xa9] = (StarMap)0x1;
  return;
}

```

## target disasm
```
  000d9b50: push {r4,r5,r6,r7,lr}
  000d9b52: add r7,sp,#0xc
  000d9b54: push.w r11
  000d9b58: mov r4,r0
  000d9b5a: ldr r0,[0x000d9b88]
  000d9b5c: movs r6,#0x1
  000d9b5e: ldr r5,[r4,#0x5c]
  000d9b60: add r0,pc
  000d9b62: strb.w r6,[r4,#0x120]
  000d9b66: mov.w r1,#0x1a6
  000d9b6a: ldr r0,[r0,#0x0]
  000d9b6c: ldr r0,[r0,#0x0]
  000d9b6e: blx 0x00072f70
  000d9b72: mov r1,r0
  000d9b74: mov r0,r5
  000d9b76: movs r2,#0x1
  000d9b78: blx 0x000746f8
  000d9b7c: strb.w r6,[r4,#0xa9]
  000d9b80: pop.w r11
  000d9b84: pop {r4,r5,r6,r7,pc}
```
