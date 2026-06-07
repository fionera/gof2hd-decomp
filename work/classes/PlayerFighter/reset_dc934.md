# PlayerFighter::reset
elf 0xdc934 body 242
Sig: undefined __thiscall reset(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::reset() */

void __thiscall PlayerFighter::reset(PlayerFighter *this)

{
  int iVar1;
  int iVar2;
  code *pcVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000eca28 + 0xec944);
  iVar1 = *piVar4;
  KIPlayer::reset((KIPlayer *)this);
  this[0x4c] = (PlayerFighter)0x1;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x158),(Vector *)&stack0xffffffd0);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x2c),(Vector *)&stack0xffffffd0);
  *(undefined4 *)(this + 0x1d0) = 0;
  *(undefined4 *)(this + 0x1d4) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x12e] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x148) = 0;
  this[300] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x1b8) = 0;
  *(undefined4 *)(this + 0x1c0) = 0;
  *(undefined4 *)(this + 0x1c4) = 0;
  *(undefined4 *)(this + 0x1c8) = 0;
  if (*(int *)(this + 0x88) != 5) {
    *(undefined4 *)(this + 0x88) = 0;
  }
  pcVar3 = *(code **)(DAT_000eca2c + 0xec9c0);
  (*pcVar3)(this + 0x90,(Vector *)&stack0xffffffd0);
  (*pcVar3)(this + 0x164,(Vector *)&stack0xffffffd0);
  (*pcVar3)(this + 0x170,(Vector *)&stack0xffffffd0);
  this[0xd0] = (PlayerFighter)0x0;
  *(undefined2 *)(this + 0x68) = 0;
  this[0x6a] = (PlayerFighter)0x0;
  this[0x4c] = (PlayerFighter)0x1;
  *(undefined4 *)(this + 0x140) = 0;
  this[0x1fc] = (PlayerFighter)0x0;
  this[0x13c] = (PlayerFighter)0x0;
  this[0x2d0] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x2c8) = 0;
  *(undefined4 *)(this + 0x2cc) = 0;
  *(undefined4 *)(this + 0x2d4) = 0;
  this[0x2d8] = (PlayerFighter)0x1;
  this[0x2e4] = (PlayerFighter)0x0;
  iVar2 = *piVar4;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  000ec934: push {r4,r5,r6,r7,lr}
  000ec936: add r7,sp,#0xc
  000ec938: push {r4,r5,r6,r7,r8,r9,r11}
  000ec93c: mov r4,r0
  000ec93e: ldr r0,[0x000eca28]
  000ec940: add r0,pc
  000ec942: ldr.w r9,[r0,#0x0]
  000ec946: ldr.w r0,[r9,#0x0]
  000ec94a: str r0,[sp,#0xc]
  000ec94c: mov r0,r4
  000ec94e: blx 0x00074518
  000ec952: mov.w r8,#0x1
  000ec956: add.w r2,r4,#0x58
  000ec95a: strb.w r8,[r4,#0x4c]
  000ec95e: ldmia r2,{r0,r1,r2}
  000ec960: stm sp,{r0,r1,r2}
  000ec964: add.w r0,r4,#0x158
  000ec968: mov r1,sp
  000ec96a: blx 0x0006eb3c
  000ec96e: ldr.w r0,[r4,#0x160]
  000ec972: mov r1,sp
  000ec974: vldr.64 d16,[r4,#0x158]
  000ec978: str r0,[sp,#0x8]
  000ec97a: add.w r0,r4,#0x2c
  000ec97e: vstr.64 d16,[sp]
  000ec982: blx 0x0006eb3c
  000ec986: ldr.w r0,[r4,#0x88]
  000ec98a: movs r6,#0x0
  000ec98c: strd r6,r6,[r4,#0x1d0]
  000ec990: str r6,[r4,#0x38]
  000ec992: cmp r0,#0x5
  000ec994: strb.w r6,[r4,#0x12e]
  000ec998: add.w r0,r4,#0x90
  000ec99c: str.w r6,[r4,#0x148]
  000ec9a0: strb.w r6,[r4,#0x12c]
  000ec9a4: str.w r6,[r4,#0x1b8]
  000ec9a8: strd r6,r6,[r4,#0x1c0]
  000ec9ac: str.w r6,[r4,#0x1c8]
  000ec9b0: it ne
  000ec9b2: str.w.ne r6,[r4,#0x88]
  000ec9b6: ldr r1,[0x000eca2c]
  000ec9b8: strd r6,r6,[sp,#0x0]
  000ec9bc: add r1,pc
  000ec9be: str r6,[sp,#0x8]
  000ec9c0: ldr r5,[r1,#0x0]
  000ec9c2: mov r1,sp
  000ec9c4: blx r5
  000ec9c6: add.w r0,r4,#0x164
  000ec9ca: mov r1,sp
  000ec9cc: strd r6,r6,[sp,#0x0]
  000ec9d0: str r6,[sp,#0x8]
  000ec9d2: blx r5
  000ec9d4: add.w r0,r4,#0x170
  000ec9d8: mov r1,sp
  000ec9da: strd r6,r6,[sp,#0x0]
  000ec9de: str r6,[sp,#0x8]
  000ec9e0: blx r5
  000ec9e2: strb.w r6,[r4,#0xd0]
  000ec9e6: strh.w r6,[r4,#0x68]
  000ec9ea: strb.w r6,[r4,#0x6a]
  000ec9ee: strb.w r8,[r4,#0x4c]
  000ec9f2: str.w r6,[r4,#0x140]
  000ec9f6: strb.w r6,[r4,#0x1fc]
  000ec9fa: strb.w r6,[r4,#0x13c]
  000ec9fe: strb.w r6,[r4,#0x2d0]
  000eca02: strd r6,r6,[r4,#0x2c8]
  000eca06: str.w r6,[r4,#0x2d4]
  000eca0a: strb.w r8,[r4,#0x2d8]
  000eca0e: strb.w r6,[r4,#0x2e4]
  000eca12: ldr r0,[sp,#0xc]
  000eca14: ldr.w r1,[r9,#0x0]
  000eca18: subs r0,r1,r0
  000eca1a: itt eq
  000eca1c: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000eca20: pop.eq {r4,r5,r6,r7,pc}
  000eca22: blx 0x0006e824
```
