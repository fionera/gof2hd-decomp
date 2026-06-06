# Status::getFreighterMissionStationBit
elf 0xacd74 body 74
Sig: undefined __stdcall getFreighterMissionStationBit(int param_1)

## decompile
```c

/* Status::getFreighterMissionStationBit(int) */

undefined4 Status::getFreighterMissionStationBit(int param_1)

{
  undefined4 uVar1;
  int in_r1;
  
  if (in_r1 == 0xf) {
    return 4;
  }
  if (in_r1 == 0x5f) {
    return 6;
  }
  if (in_r1 == 0x28) {
    return 3;
  }
  if (in_r1 == 0x2d) {
    return 9;
  }
  if (in_r1 == 0x3c) {
    return 5;
  }
  if (in_r1 == 0x46) {
    return 7;
  }
  if (in_r1 == 0x50) {
    return 8;
  }
  if (in_r1 != 0x55) {
    if (in_r1 == 0x1e) {
      uVar1 = 2;
    }
    else {
      uVar1 = 0;
    }
    return uVar1;
  }
  return 1;
}

```
## target disasm
```
  000bcd74: cmp r1,#0xf
  000bcd76: itt eq
  000bcd78: mov.eq r0,#0x4
  000bcd7a: bx.eq lr
  000bcd7c: cmp r1,#0x5f
  000bcd7e: itt eq
  000bcd80: mov.eq r0,#0x6
  000bcd82: bx.eq lr
  000bcd84: cmp r1,#0x28
  000bcd86: itt eq
  000bcd88: mov.eq r0,#0x3
  000bcd8a: bx.eq lr
  000bcd8c: cmp r1,#0x2d
  000bcd8e: itt eq
  000bcd90: mov.eq r0,#0x9
  000bcd92: bx.eq lr
  000bcd94: cmp r1,#0x3c
  000bcd96: itt eq
  000bcd98: mov.eq r0,#0x5
  000bcd9a: bx.eq lr
  000bcd9c: cmp r1,#0x46
  000bcd9e: itt eq
  000bcda0: mov.eq r0,#0x7
  000bcda2: bx.eq lr
  000bcda4: cmp r1,#0x50
  000bcda6: itt eq
  000bcda8: mov.eq r0,#0x8
  000bcdaa: bx.eq lr
  000bcdac: cmp r1,#0x55
  000bcdae: itt eq
  000bcdb0: mov.eq r0,#0x1
  000bcdb2: bx.eq lr
  000bcdb4: cmp r1,#0x1e
  000bcdb6: ite ne
  000bcdb8: mov.ne r0,#0x0
  000bcdba: mov.eq r0,#0x2
  000bcdbc: bx lr
```
