# Player::shoot
elf 0xa3f9e body 116
Sig: undefined __stdcall shoot(int param_1, int param_2, longlong param_3, bool param_4)

## decompile
```c

/* Player::shoot(int, int, long long, bool) */

void Player::shoot(int param_1,int param_2,longlong param_3,bool param_4)

{
  undefined3 in_stack_00000001;
  
  shoot(param_1,param_2,(int)param_3,_param_4,_param_4);
  return;
}

```
## target disasm
```
  000b3f9e: push {r4,r5,r6,r7,lr}
  000b3fa0: add r7,sp,#0xc
  000b3fa2: push {r8,r9,r10,r11}
  000b3fa6: sub sp,#0x5c
  000b3fa8: strd r1,r2,[sp,#0x54]
  000b3fac: add.w lr,r0,#0x20
  000b3fb0: ldr r1,[r0,#0x4]
  000b3fb2: str r1,[sp,#0x50]
  000b3fb4: ldr r1,[r0,#0x8]
  000b3fb6: str r1,[sp,#0x4c]
  000b3fb8: ldr r1,[r0,#0xc]
  000b3fba: str r1,[sp,#0x48]
  000b3fbc: ldrd r9,r10,[r0,#0x10]
  000b3fc0: ldrd r6,r3,[r0,#0x18]
  000b3fc4: ldm.w lr,{r4,r5,r11,lr}
  000b3fc8: ldrd r8,r12,[r0,#0x30]
  000b3fcc: ldrd r1,r2,[r0,#0x38]
  000b3fd0: strd r1,r2,[sp,#0x40]
  000b3fd4: add r1,sp,#0x28
  000b3fd6: strd r6,r3,[sp,#0x20]
  000b3fda: stm r1,{r4,r5,r11,lr}
  000b3fde: strd r8,r12,[sp,#0x38]
  000b3fe2: ldr r6,[sp,#0x48]
  000b3fe4: str r6,[sp,#0x14]
  000b3fe6: ldr r6,[sp,#0x4c]
  000b3fe8: str r6,[sp,#0x10]
  000b3fea: ldr r2,[r7,#0xc]
  000b3fec: ldr r6,[sp,#0x50]
  000b3fee: ldr r1,[r7,#0x10]
  000b3ff0: ldr r3,[r7,#0x8]
  000b3ff2: str.w r10,[sp,#0x1c]
  000b3ff6: str.w r9,[sp,#0x18]
  000b3ffa: strd r1,r6,[sp,#0x8]
  000b3ffe: strd r3,r2,[sp,#0x0]
  000b4002: ldrd r1,r2,[sp,#0x54]
  000b4006: blx 0x00072ff4
  000b400a: add sp,#0x5c
  000b400c: pop.w {r8,r9,r10,r11}
  000b4010: pop {r4,r5,r6,r7,pc}
```
