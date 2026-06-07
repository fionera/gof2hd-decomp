# PlayerEgo::setExhaustVisible
elf 0x9a50c body 68
Sig: undefined __thiscall setExhaustVisible(PlayerEgo * this, bool param_1)

## decompile
```c

/* PlayerEgo::setExhaustVisible(bool) */

void __thiscall PlayerEgo::setExhaustVisible(PlayerEgo *this,bool param_1)

{
  SingleLevel *pSVar1;
  uint uVar2;
  uint *puVar3;
  
  pSVar1 = this->m_pLevel;
  *(bool *)((int)&this[0x16].field_14 + 3) = param_1;
  *(bool *)pSVar1->field_80 = param_1;
  puVar3 = (uint *)pSVar1->field_A8;
  if (puVar3 != (uint *)0x0) {
    for (uVar2 = 0; uVar2 < *puVar3; uVar2 = uVar2 + 1) {
      ParticleSystemManager::enableSystemEmit
                (this->m_pLevel->field_80,SUB41(*(undefined4 *)(puVar3[1] + uVar2 * 4),0));
    }
  }
  return;
}

```

## target disasm
```
  000aa50c: push {r4,r5,r6,r7,lr}
  000aa50e: add r7,sp,#0xc
  000aa510: push.w r8
  000aa514: mov r5,r0
  000aa516: ldr r0,[r0,#0xc]
  000aa518: strb.w r1,[r5,#0x32f]
  000aa51c: mov r8,r1
  000aa51e: ldr.w r1,[r0,#0x80]
  000aa522: strb.w r8,[r1,#0x0]
  000aa526: ldr.w r6,[r0,#0xa8]
  000aa52a: cbz r6,0x000aa54a
  000aa52c: movs r4,#0x0
  000aa52e: b 0x000aa544
  000aa530: ldr r0,[r6,#0x4]
  000aa532: ldr r2,[r5,#0xc]
  000aa534: ldr.w r1,[r0,r4,lsl #0x2]
  000aa538: ldr.w r0,[r2,#0x80]
  000aa53c: mov r2,r8
  000aa53e: blx 0x000723c4
  000aa542: adds r4,#0x1
  000aa544: ldr r0,[r6,#0x0]
  000aa546: cmp r4,r0
  000aa548: bcc 0x000aa530
  000aa54a: pop.w r8
  000aa54e: pop {r4,r5,r6,r7,pc}
```
