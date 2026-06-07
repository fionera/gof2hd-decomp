# PlayerEgo::initManeuver
elf 0xa1abc body 174
Sig: undefined __thiscall initManeuver(PlayerEgo * this, int param_1)

## decompile
```c

/* PlayerEgo::initManeuver(int) */

void __thiscall PlayerEgo::initManeuver(PlayerEgo *this,int param_1)

{
  int *piVar1;
  float fVar2;
  Vector aVStack_48 [12];
  Vector aVStack_3c [12];
  Vector aVStack_30 [12];
  int local_24;
  
  piVar1 = *(int **)(DAT_000b1b74 + 0xb1ace);
  local_24 = *piVar1;
  if ((param_1 - 1U < 2) && (*(char *)&this[0x19].field_14 != '\0')) {
    *(float *)(this->m_pPlayer + 0x60) = *(float *)(this->m_pPlayer + 0x60) + DAT_000b1b6c;
  }
  if (this[0x16].field_1C == 0) {
    this[0x16].field_1C = param_1;
    this[0x17].field_14 = (int *)0x0;
    if (param_1 == 3) {
      getPosition();
      fVar2 = (float)AEGeometry::getDirection();
      AbyssEngine::AEMath::operator*(fVar2,aVStack_48);
      AbyssEngine::AEMath::operator-(aVStack_30,aVStack_3c);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&this[0x16].field_20,aVStack_30);
      AEGeometry::getDirection();
      AbyssEngine::AEMath::Vector::operator=((Vector *)&this[0x17].field_8,aVStack_30);
    }
  }
  if (*piVar1 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b1abc: push {r4,r5,r6,r7,lr}
  000b1abe: add r7,sp,#0xc
  000b1ac0: push {r8,r9,r10}
  000b1ac4: sub sp,#0x38
  000b1ac6: mov r4,r0
  000b1ac8: ldr r0,[0x000b1b74]
  000b1aca: add r0,pc
  000b1acc: ldr r6,[r0,#0x0]
  000b1ace: ldr r0,[r6,#0x0]
  000b1ad0: str r0,[sp,#0x34]
  000b1ad2: subs r0,r1,#0x1
  000b1ad4: cmp r0,#0x1
  000b1ad6: bhi 0x000b1af0
  000b1ad8: ldrb.w r0,[r4,#0x398]
  000b1adc: cbz r0,0x000b1af0
  000b1ade: ldr r0,[r4,#0x0]
  000b1ae0: vldr.32 s0,[pc,#0x88]
  000b1ae4: vldr.32 s2,[r0,#0x60]
  000b1ae8: vadd.f32 s0,s2,s0
  000b1aec: vstr.32 s0,[r0,#0x60]
  000b1af0: ldr.w r0,[r4,#0x334]
  000b1af4: cbnz r0,0x000b1b56
  000b1af6: cmp r1,#0x3
  000b1af8: mov.w r0,#0x0
  000b1afc: str.w r1,[r4,#0x334]
  000b1b00: str.w r0,[r4,#0x350]
  000b1b04: bne 0x000b1b56
  000b1b06: add.w r8,sp,#0x1c
  000b1b0a: mov r1,r4
  000b1b0c: mov r0,r8
  000b1b0e: blx 0x0007264c
  000b1b12: add.w r9,sp,#0x4
  000b1b16: ldr r1,[r4,#0x8]
  000b1b18: mov r0,r9
  000b1b1a: blx 0x00072574
  000b1b1e: add.w r10,sp,#0x10
  000b1b22: ldr r1,[0x000b1b70]
  000b1b24: mov r2,r9
  000b1b26: mov r0,r10
  000b1b28: blx 0x00072658
  000b1b2c: add r5,sp,#0x28
  000b1b2e: mov r1,r8
  000b1b30: mov r2,r10
  000b1b32: mov r0,r5
  000b1b34: blx 0x0006ec38
  000b1b38: add.w r0,r4,#0x338
  000b1b3c: mov r1,r5
  000b1b3e: blx 0x0006eb3c
  000b1b42: add r5,sp,#0x28
  000b1b44: ldr r1,[r4,#0x8]
  000b1b46: mov r0,r5
  000b1b48: blx 0x00072574
  000b1b4c: add.w r0,r4,#0x344
  000b1b50: mov r1,r5
  000b1b52: blx 0x0006eb3c
  000b1b56: ldr r0,[sp,#0x34]
  000b1b58: ldr r1,[r6,#0x0]
  000b1b5a: subs r0,r1,r0
  000b1b5c: ittt eq
  000b1b5e: add.eq sp,#0x38
  000b1b60: pop.eq.w {r8,r9,r10}
  000b1b64: pop.eq {r4,r5,r6,r7,pc}
  000b1b66: blx 0x0006e824
```
