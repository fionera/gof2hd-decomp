# Player::regenerateArmor
elf 0xa2e44 body 20
Sig: undefined __thiscall regenerateArmor(Player * this)

## decompile
```c

/* Player::regenerateArmor() */

void __thiscall Player::regenerateArmor(Player *this)

{
  int iVar1;
  
  iVar1 = *(int *)(this + 0x8c) + 2;
  if (*(int *)(this + 0x90) < *(int *)(this + 0x8c) + 2) {
    iVar1 = *(int *)(this + 0x90);
  }
  *(int *)(this + 0x8c) = iVar1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2e44: ldrd r1,r2,[r0,#0x8c]
  000b2e48: adds r1,#0x2
  000b2e4a: cmp r1,r2
  000b2e4c: it gt
  000b2e4e: mov.gt r1,r2
  000b2e50: str.w r1,[r0,#0x8c]
  000b2e54: b.w 0x001abcd8
```
