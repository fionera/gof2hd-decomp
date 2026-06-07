# ModMainMenu::ModMainMenu
elf 0x1757a0 body 30
Sig: undefined __thiscall ModMainMenu(ModMainMenu * this)

## decompile
```c

/* ModMainMenu::ModMainMenu() */

void __thiscall ModMainMenu::ModMainMenu(ModMainMenu *this)

{
  int iVar1;
  
  iVar1 = DAT_001857c0;
  this[0x10] = (ModMainMenu)0x0;
  this[0x29] = (ModMainMenu)0x0;
  *(undefined4 *)(this + 0xc) = 100;
  iVar1 = *(int *)(iVar1 + 0x1857ac);
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(int *)this = iVar1 + 8;
  return;
}

```

## target disasm
```
  001857a0: ldr r1,[0x001857c0]
  001857a2: movs r2,#0x0
  001857a4: movs r3,#0x64
  001857a6: strb r2,[r0,#0x10]
  001857a8: add r1,pc
  001857aa: strb.w r2,[r0,#0x29]
  001857ae: str r3,[r0,#0xc]
  001857b0: ldr r1,[r1,#0x0]
  001857b2: strd r2,r2,[r0,#0x14]
  001857b6: str r2,[r0,#0x1c]
  001857b8: adds r1,#0x8
  001857ba: str r1,[r0,#0x0]
  001857bc: bx lr
```
