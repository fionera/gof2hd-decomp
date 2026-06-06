# Player::reset
elf 0xa2a70 body 88
Sig: undefined __stdcall reset(void)

## decompile
```c

/* Player::reset() */

void Player::reset(void)

{
  Player *in_r0;
  uint in_fpscr;
  undefined4 uVar1;
  
  uVar1 = VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x94),(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(in_r0 + 0xb8) = DAT_000b2ac8;
  *(undefined4 *)(in_r0 + 0x78) = *(undefined4 *)(in_r0 + 0x84);
  *(undefined4 *)(in_r0 + 0x7c) = *(undefined4 *)(in_r0 + 0x80);
  *(undefined4 *)(in_r0 + 0x88) = uVar1;
  updateDamageRate(in_r0);
  in_r0[0xc2] = (Player)0x1;
  *(undefined2 *)(in_r0 + 0xc0) = 1;
  *(undefined2 *)(in_r0 + 0x54) = 0;
  in_r0[0x44] = (Player)0x0;
  *(undefined4 *)(in_r0 + 0x6c) = 0;
  in_r0[0x5e] = (Player)0x0;
  *(undefined4 *)(in_r0 + 0xb4) = 0;
  in_r0[0x68] = (Player)0x0;
  *(undefined4 *)(in_r0 + 100) = 0;
  *(undefined4 *)(in_r0 + 0xd4) = 0;
  *(undefined4 *)(in_r0 + 0xd8) = 0;
  *(undefined4 *)(in_r0 + 0xdd) = 0;
  *(undefined4 *)(in_r0 + 0xd9) = 0;
  return;
}

```
## target disasm
```
  000b2a70: push {r4,r6,r7,lr}
  000b2a72: add r7,sp,#0x8
  000b2a74: vldr.32 s0,[r0,#0x94]
  000b2a78: mov r4,r0
  000b2a7a: ldr r0,[0x000b2ac8]
  000b2a7c: vcvt.f32.s32 s0,s0
  000b2a80: ldrd r1,r2,[r4,#0x80]
  000b2a84: str.w r0,[r4,#0xb8]
  000b2a88: mov r0,r4
  000b2a8a: strd r2,r1,[r4,#0x78]
  000b2a8e: vstr.32 s0,[r4,#0x88]
  000b2a92: blx 0x00072df0
  000b2a96: movs r0,#0x1
  000b2a98: strb.w r0,[r4,#0xc2]
  000b2a9c: strh.w r0,[r4,#0xc0]
  000b2aa0: movs r0,#0x0
  000b2aa2: strh.w r0,[r4,#0x54]
  000b2aa6: strb.w r0,[r4,#0x44]
  000b2aaa: str r0,[r4,#0x6c]
  000b2aac: strb.w r0,[r4,#0x5e]
  000b2ab0: str.w r0,[r4,#0xb4]
  000b2ab4: strb.w r0,[r4,#0x68]
  000b2ab8: str r0,[r4,#0x64]
  000b2aba: strd r0,r0,[r4,#0xd4]
  000b2abe: str.w r0,[r4,#0xdd]
  000b2ac2: str.w r0,[r4,#0xd9]
  000b2ac6: pop {r4,r6,r7,pc}
```
