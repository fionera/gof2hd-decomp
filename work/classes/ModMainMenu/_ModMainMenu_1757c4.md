# ModMainMenu::~ModMainMenu
elf 0x1757c4 body 26
Sig: undefined __thiscall ~ModMainMenu(ModMainMenu * this)

## decompile
```c

/* ModMainMenu::~ModMainMenu() */

ModMainMenu * __thiscall ModMainMenu::~ModMainMenu(ModMainMenu *this)

{
  *(int *)this = *(int *)(DAT_001857e4 + 0x1857d0) + 8;
  OnRelease(this);
  return this;
}

```

## target disasm
```
  001857c4: push {r4,r6,r7,lr}
  001857c6: add r7,sp,#0x8
  001857c8: mov r4,r0
  001857ca: ldr r0,[0x001857e4]
  001857cc: add r0,pc
  001857ce: ldr r0,[r0,#0x0]
  001857d0: adds r0,#0x8
  001857d2: str r0,[r4,#0x0]
  001857d4: mov r0,r4
  001857d6: blx 0x00077eb4
  001857da: mov r0,r4
  001857dc: pop {r4,r6,r7,pc}
```
