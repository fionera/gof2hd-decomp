# KIPlayer::translate
elf 0xa6332 body 52
Sig: undefined __thiscall translate(KIPlayer * this, Vector * param_1)

## decompile
```c

/* KIPlayer::translate(AbyssEngine::AEMath::Vector const&) */

void __thiscall KIPlayer::translate(KIPlayer *this,Vector *param_1)

{
  AEGeometry::translate(*(Vector **)(this + 8));
  if (*(int *)(this + 0x6c) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001abe20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001abe24 + 0x1abe28))(*(int *)(this + 0x6c),param_1);
  return;
}

```

## target disasm
```
  00073348: adr r12,0x173350
  0007334c: add r12,r12,#0xae000
  00073350: ldr pc,[r12,#0xc60]!
  000b6332: push {r4,r5,r7,lr}
  000b6334: add r7,sp,#0x8
  000b6336: mov r5,r0
  000b6338: ldr r0,[r0,#0x8]
  000b633a: mov r4,r1
  000b633c: blx 0x00072460
  000b6340: ldr r0,[r5,#0x6c]
  000b6342: cbz r0,0x000b634e
  000b6344: mov r1,r4
  000b6346: pop.w {r4,r5,r7,lr}
  000b634a: b.w 0x001abe18
  000b634e: pop {r4,r5,r7,pc}
  001abe18: bx pc
  001abe1c: ldr r12,[0x1abe24]
  001abe20: add pc,r12,pc
```
