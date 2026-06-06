# Status::isFreighterMissionStation
elf 0xacd2c body 62
Sig: undefined __stdcall isFreighterMissionStation(int param_1)

## decompile
```c

/* Status::isFreighterMissionStation(int) */

undefined4 Status::isFreighterMissionStation(int param_1)

{
  int in_r1;
  
  if ((in_r1 - 0x1eU < 0x1f) && ((1 << (in_r1 - 0x1eU & 0xff) & DAT_000bcd6c) != 0)) {
    return 1;
  }
  if ((in_r1 - 0x46U < 0x1a) && ((1 << (in_r1 - 0x46U & 0xff) & DAT_000bcd70) != 0)) {
    return 1;
  }
  if (in_r1 == 0xf) {
    return 1;
  }
  return 0;
}

```
## target disasm
```
  000bcd2c: sub.w r2,r1,#0x1e
  000bcd30: movs r0,#0x1
  000bcd32: cmp r2,#0x1e
  000bcd34: bhi 0x000bcd42
  000bcd36: ldr r3,[0x000bcd6c]
  000bcd38: lsl.w r2,r0,r2
  000bcd3c: tst r2,r3
  000bcd3e: it ne
  000bcd40: bx.ne lr
  000bcd42: sub.w r2,r1,#0x46
  000bcd46: cmp r2,#0x19
  000bcd48: bhi 0x000bcd60
  000bcd4a: push {r4,r6,r7,lr}
  000bcd4c: add r7,sp,#0x8
  000bcd4e: ldr r4,[0x000bcd70]
  000bcd50: movs r3,#0x1
  000bcd52: lsl.w r2,r3,r2
  000bcd56: tst r2,r4
  000bcd58: pop.w {r4,r6,r7,lr}
  000bcd5c: beq 0x000bcd60
  000bcd5e: bx lr
  000bcd60: cmp r1,#0xf
  000bcd62: itt ne
  000bcd64: mov.ne r0,#0x0
  000bcd66: bx.ne lr
  000bcd68: bx lr
```
