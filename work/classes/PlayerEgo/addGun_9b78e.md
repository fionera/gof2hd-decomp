# PlayerEgo::addGun
elf 0x9b78e body 24
Sig: undefined __stdcall addGun(Array * param_1, int param_2)

## decompile
```c

/* PlayerEgo::addGun(Array<Gun*>*, int) */

void PlayerEgo::addGun(Array *param_1,int param_2)

{
  int in_r2;
  
  Player::addGun(*(Player **)param_1,(Array *)param_2,in_r2);
  (*(code *)(DAT_001abaa4 + 0x1abaa8))(param_1);
  return;
}

```

## target disasm
```
  000ab78e: push {r4,r6,r7,lr}
  000ab790: add r7,sp,#0x8
  000ab792: mov r4,r0
  000ab794: ldr r0,[r0,#0x0]
  000ab796: blx 0x00072544
  000ab79a: mov r0,r4
  000ab79c: pop.w {r4,r6,r7,lr}
  000ab7a0: b.w 0x001aba98
  001aba98: bx pc
```
