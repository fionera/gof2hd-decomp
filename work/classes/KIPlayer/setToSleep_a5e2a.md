# KIPlayer::setToSleep
elf 0xa5e2a body 28
Sig: undefined __thiscall setToSleep(KIPlayer * this)

## decompile
```c

/* KIPlayer::setToSleep() */

void __thiscall KIPlayer::setToSleep(KIPlayer *this)

{
  *(undefined4 *)(this + 0x88) = 5;
  Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
  this[0xb1] = (KIPlayer)0x1;
  return;
}

```

## target disasm
```
  000b5e2a: push {r4,r6,r7,lr}
  000b5e2c: add r7,sp,#0x8
  000b5e2e: mov r4,r0
  000b5e30: ldr r0,[r0,#0x4]
  000b5e32: movs r1,#0x5
  000b5e34: str.w r1,[r4,#0x88]
  000b5e38: movs r1,#0x0
  000b5e3a: blx 0x00072580
  000b5e3e: movs r0,#0x1
  000b5e40: strb.w r0,[r4,#0xb1]
  000b5e44: pop {r4,r6,r7,pc}
```
