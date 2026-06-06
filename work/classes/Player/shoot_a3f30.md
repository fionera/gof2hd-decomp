# Player::shoot
elf 0xa3f30 body 110
Sig: undefined __stdcall shoot(int param_1, longlong param_2, bool param_3)

## decompile
```c

/* Player::shoot(int, long long, bool) */

undefined4 Player::shoot(int param_1,longlong param_2,bool param_3)

{
  undefined4 in_r1;
  undefined3 in_stack_00000001;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  
  uStack_2c = (undefined4)param_2;
  uStack_28 = (undefined4)((ulonglong)param_2 >> 0x20);
  shoot(param_1,in_r1,uStack_2c,uStack_28,_param_3,*(undefined4 *)(param_1 + 4),
        *(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0xc),*(undefined4 *)(param_1 + 0x10),
        *(undefined4 *)(param_1 + 0x14),*(undefined4 *)(param_1 + 0x18),
        *(undefined4 *)(param_1 + 0x1c),*(undefined4 *)(param_1 + 0x20),
        *(undefined4 *)(param_1 + 0x24),*(undefined4 *)(param_1 + 0x28),
        *(undefined4 *)(param_1 + 0x2c),*(undefined4 *)(param_1 + 0x30),
        *(undefined4 *)(param_1 + 0x34),*(undefined4 *)(param_1 + 0x38),
        *(undefined4 *)(param_1 + 0x3c));
  return 1;
}

```
## target disasm
```
  000b3f30: push {r4,r5,r6,r7,lr}
  000b3f32: add r7,sp,#0xc
  000b3f34: push {r8,r9,r10,r11}
  000b3f38: sub sp,#0x5c
  000b3f3a: add.w r12,sp,#0x50
  000b3f3e: add.w r11,r0,#0x10
  000b3f42: stm.w r12!,{r1,r2,r3}
  000b3f46: add.w lr,r0,#0x2c
  000b3f4a: ldr r1,[r0,#0x4]
  000b3f4c: str r1,[sp,#0x4c]
  000b3f4e: ldr r1,[r0,#0x8]
  000b3f50: str r1,[sp,#0x48]
  000b3f52: ldr r1,[r0,#0xc]
  000b3f54: str r1,[sp,#0x44]
  000b3f56: ldm.w r11,{r9,r10,r11}
  000b3f5a: ldrd r8,r6,[r0,#0x1c]
  000b3f5e: ldrd r4,r1,[r0,#0x24]
  000b3f62: ldm.w lr,{r2,r3,r12,lr}
  000b3f66: ldr r5,[r0,#0x3c]
  000b3f68: strd r6,r4,[sp,#0x20]
  000b3f6c: add r4,sp,#0x28
  000b3f6e: stm r4,{r1,r2,r3,r12,lr}
  000b3f72: add r3,sp,#0x50
  000b3f74: str r5,[sp,#0x3c]
  000b3f76: strd r11,r8,[sp,#0x18]
  000b3f7a: strd r9,r10,[sp,#0x10]
  000b3f7e: ldr r2,[sp,#0x44]
  000b3f80: str r2,[sp,#0xc]
  000b3f82: ldr r2,[sp,#0x48]
  000b3f84: str r2,[sp,#0x8]
  000b3f86: ldr r2,[sp,#0x4c]
  000b3f88: ldr r1,[r7,#0x8]
  000b3f8a: strd r1,r2,[sp,#0x0]
  000b3f8e: ldmia r3,{r1,r2,r3}
  000b3f90: blx 0x000724fc
  000b3f94: movs r0,#0x1
  000b3f96: add sp,#0x5c
  000b3f98: pop.w {r8,r9,r10,r11}
  000b3f9c: pop {r4,r5,r6,r7,pc}
```
