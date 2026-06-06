# Player::regenerateHull
elf 0xa2e58 body 20
Sig: undefined __thiscall regenerateHull(Player * this)

## decompile
```c

/* Player::regenerateHull() */

void __thiscall Player::regenerateHull(Player *this)

{
  int iVar1;
  
  iVar1 = *(int *)(this + 0x84);
  if (*(int *)(this + 0x78) + 1 < *(int *)(this + 0x84)) {
    iVar1 = *(int *)(this + 0x78) + 1;
  }
  *(int *)(this + 0x78) = iVar1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2e58: ldr r1,[r0,#0x78]
  000b2e5a: ldr.w r2,[r0,#0x84]
  000b2e5e: adds r1,#0x1
  000b2e60: cmp r1,r2
  000b2e62: it lt
  000b2e64: mov.lt r2,r1
  000b2e66: str r2,[r0,#0x78]
  000b2e68: b.w 0x001abcd8
```
