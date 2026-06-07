# ModMainMenu::~ModMainMenu
elf 0x1757e8 body 16
Sig: undefined __thiscall ~ModMainMenu(ModMainMenu * this)

## decompile
```c

/* ModMainMenu::~ModMainMenu() */

void __thiscall ModMainMenu::~ModMainMenu(ModMainMenu *this)

{
  ~ModMainMenu(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  001857e8: push {r7,lr}
  001857ea: mov r7,sp
  001857ec: blx 0x00077ec0
  001857f0: pop.w {r7,lr}
  001857f4: b.w 0x001ab098
```
