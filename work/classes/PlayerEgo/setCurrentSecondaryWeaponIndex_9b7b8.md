# PlayerEgo::setCurrentSecondaryWeaponIndex
elf 0x9b7b8 body 18
Sig: undefined __stdcall setCurrentSecondaryWeaponIndex(int param_1)

## decompile
```c

/* PlayerEgo::setCurrentSecondaryWeaponIndex(int) */

void PlayerEgo::setCurrentSecondaryWeaponIndex(int param_1)

{
  int iVar1;
  undefined4 in_r1;
  
  iVar1 = DAT_000ab7cc;
  *(undefined4 *)(param_1 + 0x10c) = in_r1;
  *(undefined4 *)(**(int **)(iVar1 + 0xab7c2) + 0xf4) = in_r1;
  return;
}

```

## target disasm
```
  000ab7b8: ldr r2,[0x000ab7cc]
  000ab7ba: str.w r1,[r0,#0x10c]
  000ab7be: add r2,pc
  000ab7c0: ldr r2,[r2,#0x0]
  000ab7c2: ldr r0,[r2,#0x0]
  000ab7c4: str.w r1,[r0,#0xf4]
  000ab7c8: bx lr
```
