# KIPlayer::reset
elf 0xa5de6 body 68
Sig: undefined __thiscall reset(KIPlayer * this)

## decompile
```c

/* KIPlayer::reset() */

void __thiscall KIPlayer::reset(KIPlayer *this)

{
  if (*(int *)(this + 4) != 0) {
    Player::reset();
  }
  if ((this[0xb1] != (KIPlayer)0x0) || (this[0xb2] == (KIPlayer)0x0)) {
    setToSleep(this);
  }
  if (*(int *)(this + 0xb4) == 0) {
    if (*(int *)(this + 0x6c) != 0) {
      Route::reset();
    }
  }
  else {
    *(int *)(this + 0x6c) = *(int *)(this + 0xb4);
    Route::reset();
  }
  this[0x100] = (KIPlayer)0x0;
  *(undefined4 *)(this + 0xfc) = 0;
  return;
}

```

## target disasm
```
  000b5de6: push {r4,r6,r7,lr}
  000b5de8: add r7,sp,#0x8
  000b5dea: mov r4,r0
  000b5dec: ldr r0,[r0,#0x4]
  000b5dee: cbz r0,0x000b5df4
  000b5df0: blx 0x000732d0
  000b5df4: ldrb.w r0,[r4,#0xb1]
  000b5df8: cbnz r0,0x000b5e00
  000b5dfa: ldrb.w r0,[r4,#0xb2]
  000b5dfe: cbnz r0,0x000b5e06
  000b5e00: mov r0,r4
  000b5e02: blx 0x000732dc
  000b5e06: ldr.w r0,[r4,#0xb4]
  000b5e0a: cbz r0,0x000b5e14
  000b5e0c: str r0,[r4,#0x6c]
  000b5e0e: blx 0x000732e8
  000b5e12: b 0x000b5e1e
  000b5e14: ldr r0,[r4,#0x6c]
  000b5e16: cmp r0,#0x0
  000b5e18: it ne
  000b5e1a: blx.ne 0x000732e8
  000b5e1e: movs r0,#0x0
  000b5e20: strb.w r0,[r4,#0x100]
  000b5e24: str.w r0,[r4,#0xfc]
  000b5e28: pop {r4,r6,r7,pc}
```
