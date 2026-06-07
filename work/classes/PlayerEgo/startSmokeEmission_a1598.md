# PlayerEgo::startSmokeEmission
elf 0xa1598 body 46
Sig: undefined __thiscall startSmokeEmission(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::startSmokeEmission() */

void __thiscall PlayerEgo::startSmokeEmission(PlayerEgo *this)

{
  if ((int)this[0x15].m_pLevel < 0) {
    return;
  }
  ParticleSystemManager::enableSystemEmit(this->m_pLevel->field_78,SUB41(this[0x15].m_pLevel,0));
  (*(code *)(DAT_001aba84 + 0x1aba88))(this->m_pLevel->field_84,this[0x15].field_10,1);
  return;
}

```

## target disasm
```
  000b1598: push {r4,r6,r7,lr}
  000b159a: add r7,sp,#0x8
  000b159c: ldr.w r1,[r0,#0x300]
  000b15a0: mov r4,r0
  000b15a2: cmp r1,#0x0
  000b15a4: blt 0x000b15c4
  000b15a6: ldr r0,[r4,#0xc]
  000b15a8: movs r2,#0x1
  000b15aa: ldr r0,[r0,#0x78]
  000b15ac: blx 0x000723c4
  000b15b0: ldr r0,[r4,#0xc]
  000b15b2: movs r2,#0x1
  000b15b4: ldr.w r1,[r4,#0x304]
  000b15b8: ldr.w r0,[r0,#0x84]
  000b15bc: pop.w {r4,r6,r7,lr}
  000b15c0: b.w 0x001aba78
  000b15c4: pop {r4,r6,r7,pc}
```
