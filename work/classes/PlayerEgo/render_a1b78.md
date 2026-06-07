# PlayerEgo::render
elf 0xa1b78 body 224
Sig: undefined __stdcall render(bool param_1)

## decompile
```c

/* PlayerEgo::render(bool) */

void PlayerEgo::render(bool param_1)

{
  PlayerEgo *this;
  int iVar1;
  SingleLevel *pSVar2;
  uint *puVar3;
  int in_r1;
  undefined4 uVar4;
  uint uVar5;
  
  this = (PlayerEgo *)(uint)param_1;
  iVar1 = isDead(this);
  if (iVar1 == 0) {
    if (*(char *)((int)&this[0x15].field_14 + 1) == '\0') {
      return;
    }
    if (this[3].field_20 != 0) {
      Explosion::render();
    }
    AEGeometry::render();
    if ((this[4].field_1C != 0) && (0 < (int)this[0x15].field_18)) {
      AEGeometry::render();
    }
    if (*(char *)&this[1].field_14 != '\0') {
      AEGeometry::render();
    }
    if (*(char *)&this[10].field_8 != '\0') {
      if (this[1].m_pLevel != (SingleLevel *)0x0) {
        AEGeometry::setVisible(SUB41(this[1].m_pLevel,0));
      }
      AEGeometry::render();
    }
    if ((TractorBeam *)this[0xc].field_4 != (TractorBeam *)0x0) {
      TractorBeam::render((TractorBeam *)this[0xc].field_4);
    }
    puVar3 = (uint *)this[0xc].field_8;
    if (puVar3 != (uint *)0x0) {
      for (uVar5 = 0; uVar5 < *puVar3; uVar5 = uVar5 + 1) {
        RepairBeam::render(*(RepairBeam **)(puVar3[1] + uVar5 * 4));
        puVar3 = (uint *)this[0xc].field_8;
      }
    }
    uVar4 = 0;
    pSVar2 = this->m_pLevel;
    if ((((char)this[1].m_pPlayer == '\0') && (in_r1 != 0)) &&
       (uVar4 = 0, this[0xc].field_14 != (int *)0x1)) {
      uVar4 = 1;
    }
  }
  else {
    if ((this[3].field_20 != 0) && (Explosion::render(), (int)this[0x15].field_4 < 3000)) {
      AEGeometry::render();
    }
    if (this[4].m_pPlayer != 0) {
      Explosion::render();
    }
    pSVar2 = this->m_pLevel;
    uVar4 = 1;
  }
  (*(code *)(DAT_001abc44 + 0x1abc48))(pSVar2,uVar4);
  return;
}

```

## target disasm
```
  000b1b78: push {r4,r5,r6,r7,lr}
  000b1b7a: add r7,sp,#0xc
  000b1b7c: push.w r11
  000b1b80: mov r5,r1
  000b1b82: mov r4,r0
  000b1b84: blx 0x000724b4
  000b1b88: cbz r0,0x000b1bb6
  000b1b8a: ldr.w r0,[r4,#0x8c]
  000b1b8e: cbz r0,0x000b1ba6
  000b1b90: blx 0x00072d18
  000b1b94: ldr.w r0,[r4,#0x2f8]
  000b1b98: movw r1,#0xbb7
  000b1b9c: cmp r0,r1
  000b1b9e: itt le
  000b1ba0: ldr.le r0,[r4,#0x8]
  000b1ba2: blx.le 0x00072238
  000b1ba6: ldr.w r0,[r4,#0x90]
  000b1baa: cbz r0,0x000b1bb0
  000b1bac: blx 0x00072d18
  000b1bb0: ldr r0,[r4,#0xc]
  000b1bb2: movs r1,#0x1
  000b1bb4: b 0x000b1c44
  000b1bb6: ldrb.w r0,[r4,#0x309]
  000b1bba: cmp r0,#0x0
  000b1bbc: beq 0x000b1c50
  000b1bbe: ldr.w r0,[r4,#0x8c]
  000b1bc2: cbz r0,0x000b1bc8
  000b1bc4: blx 0x00072d18
  000b1bc8: ldr r0,[r4,#0x8]
  000b1bca: blx 0x00072238
  000b1bce: ldr.w r0,[r4,#0xac]
  000b1bd2: cbz r0,0x000b1be0
  000b1bd4: ldr.w r1,[r4,#0x30c]
  000b1bd8: cmp r1,#0x1
  000b1bda: it ge
  000b1bdc: blx.ge 0x00072238
  000b1be0: ldrb.w r0,[r4,#0x38]
  000b1be4: cbz r0,0x000b1bec
  000b1be6: ldr r0,[r4,#0x34]
  000b1be8: blx 0x00072238
  000b1bec: ldrb.w r0,[r4,#0x170]
  000b1bf0: cbz r0,0x000b1c04
  000b1bf2: ldr r0,[r4,#0x30]
  000b1bf4: cbz r0,0x000b1bfe
  000b1bf6: ldrb.w r1,[r4,#0x1a0]
  000b1bfa: blx 0x00072d24
  000b1bfe: ldr r0,[r4,#0x2c]
  000b1c00: blx 0x00072238
  000b1c04: ldr.w r0,[r4,#0x1b4]
  000b1c08: cbz r0,0x000b1c0e
  000b1c0a: blx 0x00072d30
  000b1c0e: ldr.w r0,[r4,#0x1b8]
  000b1c12: cbz r0,0x000b1c2e
  000b1c14: movs r6,#0x0
  000b1c16: b 0x000b1c28
  000b1c18: ldr r0,[r0,#0x4]
  000b1c1a: ldr.w r0,[r0,r6,lsl #0x2]
  000b1c1e: blx 0x00072d3c
  000b1c22: ldr.w r0,[r4,#0x1b8]
  000b1c26: adds r6,#0x1
  000b1c28: ldr r1,[r0,#0x0]
  000b1c2a: cmp r6,r1
  000b1c2c: bcc 0x000b1c18
  000b1c2e: ldrb.w r2,[r4,#0x24]
  000b1c32: movs r1,#0x0
  000b1c34: ldr r0,[r4,#0xc]
  000b1c36: cbnz r2,0x000b1c44
  000b1c38: cbz r5,0x000b1c44
  000b1c3a: ldr.w r1,[r4,#0x1c4]
  000b1c3e: subs r1,#0x1
  000b1c40: it ne
  000b1c42: mov.ne r1,#0x1
  000b1c44: pop.w r11
  000b1c48: pop.w {r4,r5,r6,r7,lr}
  000b1c4c: b.w 0x001abc38
  000b1c50: pop.w r11
  000b1c54: pop {r4,r5,r6,r7,pc}
  001abc38: bx pc
```
