# Player::ResumeEngineSound
elf 0xa40ac body 46
Sig: undefined __thiscall ResumeEngineSound(Player * this, bool param_1)

## decompile
```c

/* Player::ResumeEngineSound(bool) */

void __thiscall Player::ResumeEngineSound(Player *this,bool param_1)

{
  byte bVar1;
  
  if ((*(Event **)(this + 0xf0) != (Event *)0x0) && ((this[0xf8] != (Player)0x0 || (param_1)))) {
    bVar1 = FModSound::resume((FModSound *)**(undefined4 **)(DAT_000b40dc + 0xb40ca),
                              *(Event **)(this + 0xf0));
    this[0xf8] = (Player)(bVar1 ^ 1);
    return;
  }
  return;
}

```
## target disasm
```
  000b40ac: push {r4,r6,r7,lr}
  000b40ae: add r7,sp,#0x8
  000b40b0: mov r2,r1
  000b40b2: ldr.w r1,[r0,#0xf0]
  000b40b6: mov r4,r0
  000b40b8: cbz r1,0x000b40c2
  000b40ba: ldrb.w r0,[r4,#0xf8]
  000b40be: cbnz r0,0x000b40c4
  000b40c0: cbnz r2,0x000b40c4
  000b40c2: pop {r4,r6,r7,pc}
  000b40c4: ldr r0,[0x000b40dc]
  000b40c6: add r0,pc
  000b40c8: ldr r0,[r0,#0x0]
  000b40ca: ldr r0,[r0,#0x0]
  000b40cc: blx 0x0007300c
  000b40d0: eor r0,r0,#0x1
  000b40d4: strb.w r0,[r4,#0xf8]
  000b40d8: pop {r4,r6,r7,pc}
```
