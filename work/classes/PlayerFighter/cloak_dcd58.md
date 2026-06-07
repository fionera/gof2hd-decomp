# PlayerFighter::cloak
elf 0xdcd58 body 62
Sig: undefined __stdcall cloak(int param_1, bool param_2)

## decompile
```c

/* PlayerFighter::cloak(int, bool) */

void PlayerFighter::cloak(int param_1,bool param_2)

{
  int iVar1;
  uint uVar2;
  undefined1 in_r2;
  
  uVar2 = (uint)param_2;
  if (uVar2 == 0) {
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000ecd98 + 0xecd76));
    uVar2 = iVar1 + 5000;
  }
  *(undefined1 *)(param_1 + 0x2d0) = 1;
  *(uint *)(param_1 + 0x2cc) = uVar2 + 4000;
  *(undefined1 *)(param_1 + 0x2d9) = in_r2;
  return;
}

```

## target disasm
```
  000ecd58: push {r4,r5,r6,r7,lr}
  000ecd5a: add r7,sp,#0xc
  000ecd5c: push.w r11
  000ecd60: mov r4,r2
  000ecd62: mov r5,r0
  000ecd64: cmp r1,#0x0
  000ecd66: bgt 0x000ecd7e
  000ecd68: ldr r0,[0x000ecd98]
  000ecd6a: movw r1,#0x1388
  000ecd6e: movw r6,#0x1388
  000ecd72: add r0,pc
  000ecd74: ldr r0,[r0,#0x0]
  000ecd76: ldr r0,[r0,#0x0]
  000ecd78: blx 0x00071848
  000ecd7c: adds r1,r0,r6
  000ecd7e: movs r0,#0x1
  000ecd80: strb.w r0,[r5,#0x2d0]
  000ecd84: add.w r0,r1,#0xfa0
  000ecd88: str.w r0,[r5,#0x2cc]
  000ecd8c: strb.w r4,[r5,#0x2d9]
  000ecd90: pop.w r11
  000ecd94: pop {r4,r5,r6,r7,pc}
```
