# PlayerTurret::revive
elf 0x157c08 body 76
Sig: undefined __thiscall revive(PlayerTurret * this)

## decompile
```c

/* PlayerTurret::revive() */

void __thiscall PlayerTurret::revive(PlayerTurret *this)

{
  int iVar1;
  
  Player::reset();
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x88) = 1;
  *(undefined4 *)(this + 0xd8) = 0;
  KIPlayer::setActive(SUB41(this,0));
  Explosion::reset(*(Explosion **)(this + 0x13c));
  iVar1 = *(int *)(this + 0xc);
  *(undefined4 *)(this + 0x128) = 0;
  this[0xf5] = (PlayerTurret)0x1;
  if (iVar1 == 0) {
    iVar1 = *(int *)(this + 8);
  }
  (*(code *)(DAT_001abde4 + 0x1abde8))(iVar1,1);
  return;
}

```

## target disasm
```
  00167c08: push {r4,r5,r6,r7,lr}
  00167c0a: add r7,sp,#0xc
  00167c0c: push.w r11
  00167c10: mov r4,r0
  00167c12: ldr r0,[r0,#0x4]
  00167c14: blx 0x000732d0
  00167c18: movs r5,#0x0
  00167c1a: movs r6,#0x1
  00167c1c: mov r0,r4
  00167c1e: movs r1,#0x1
  00167c20: str r5,[r4,#0x78]
  00167c22: str.w r6,[r4,#0x88]
  00167c26: str.w r5,[r4,#0xd8]
  00167c2a: blx 0x000732f4
  00167c2e: ldr.w r0,[r4,#0x13c]
  00167c32: blx 0x00072c88
  00167c36: ldr r0,[r4,#0xc]
  00167c38: str.w r5,[r4,#0x128]
  00167c3c: cmp r0,#0x0
  00167c3e: strb.w r6,[r4,#0xf5]
  00167c42: bne 0x00167c46
  00167c44: ldr r0,[r4,#0x8]
  00167c46: movs r1,#0x1
  00167c48: pop.w r11
  00167c4c: pop.w {r4,r5,r6,r7,lr}
  00167c50: b.w 0x001abdd8
```
