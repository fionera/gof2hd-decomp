# PlayerEgo::addGun
elf 0x9b778 body 22
Sig: undefined __stdcall addGun(Gun * param_1, int param_2)

## decompile
```c

/* PlayerEgo::addGun(Gun*, int) */

void PlayerEgo::addGun(Gun *param_1,int param_2)

{
  int in_r2;
  
  Player::addGun(*(Player **)param_1,(Gun *)param_2,in_r2);
  (*(code *)(DAT_001abaa4 + 0x1abaa8))(param_1);
  return;
}

```

## target disasm
```
  000ab778: push {r4,r6,r7,lr}
  000ab77a: add r7,sp,#0x8
  000ab77c: mov r4,r0
  000ab77e: ldr r0,[r0,#0x0]
  000ab780: blx 0x0007252c
  000ab784: mov r0,r4
  000ab786: pop.w {r4,r6,r7,lr}
  000ab78a: b.w 0x001aba98
```
