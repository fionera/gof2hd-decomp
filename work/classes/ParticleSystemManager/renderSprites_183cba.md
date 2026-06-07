# ParticleSystemManager::renderSprites
elf 0x183cba body 48
Sig: undefined __thiscall renderSprites(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::renderSprites() */

void __thiscall ParticleSystemManager::renderSprites(ParticleSystemManager *this)

{
  if (*(short *)(this + 0x24) != -1) {
                    /* WARNING: Could not recover jumptable at 0x001ac9a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(DAT_001ac9a4 + 0x1ac9a8))(*(undefined4 *)(this + 4),*(undefined4 *)(this + 0x30));
    return;
  }
  if (*(short *)(this + 0x26) != -1) {
                    /* WARNING: Could not recover jumptable at 0x001ac990. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(DAT_001ac994 + 0x1ac998))
              (*(undefined4 *)(this + 4),*(undefined4 *)(this + 0x30),*(undefined4 *)(this + 0x2c),
               *(undefined4 *)(this + 0x28));
    return;
  }
  return;
}

```

## target disasm
```
  00193cba: ldrh r2,[r0,#0x24]
  00193cbc: movw r1,#0xffff
  00193cc0: cmp r2,r1
  00193cc2: bne 0x00193cdc
  00193cc4: ldrh r2,[r0,#0x26]
  00193cc6: cmp r2,r1
  00193cc8: it eq
  00193cca: bx.eq lr
  00193ccc: ldr.w r12,[r0,#0x4]
  00193cd0: ldrd r3,r2,[r0,#0x28]
  00193cd4: ldr r1,[r0,#0x30]
  00193cd6: mov r0,r12
  00193cd8: b.w 0x001ac988
  00193cdc: ldr r2,[r0,#0x4]
  00193cde: ldr r1,[r0,#0x30]
  00193ce0: mov r0,r2
  00193ce2: b.w 0x001ac998
  001ac988: bx pc
  001ac998: bx pc
```
