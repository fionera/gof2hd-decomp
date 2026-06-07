# PlayerEgo::getCurrentSecondaryWeaponIndex
elf 0x9b7b0 body 6
Sig: undefined __stdcall getCurrentSecondaryWeaponIndex(void)

## decompile
```c

/* PlayerEgo::getCurrentSecondaryWeaponIndex() */

undefined4 PlayerEgo::getCurrentSecondaryWeaponIndex(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x10c);
}

```

## target disasm
```
  000ab7b0: ldr.w r0,[r0,#0x10c]
  000ab7b4: bx lr
```
