# Player::PauseEngineSound
elf 0xa4088 body 30
Sig: undefined __thiscall PauseEngineSound(Player * this)

## decompile
```c

/* Player::PauseEngineSound() */

void __thiscall Player::PauseEngineSound(Player *this)

{
  int iVar1;
  
  if (*(int *)(this + 0xf0) != 0) {
    iVar1 = FModSound::pause();
    this[0xf8] = SUB41(iVar1,0);
  }
  return;
}

```
## target disasm
```
  000b4088: push {r4,r6,r7,lr}
  000b408a: add r7,sp,#0x8
  000b408c: ldr.w r1,[r0,#0xf0]
  000b4090: mov r4,r0
  000b4092: cbz r1,0x000b40a4
  000b4094: ldr r0,[0x000b40a8]
  000b4096: add r0,pc
  000b4098: ldr r0,[r0,#0x0]
  000b409a: ldr r0,[r0,#0x0]
  000b409c: blx 0x00073000
  000b40a0: strb.w r0,[r4,#0xf8]
  000b40a4: pop {r4,r6,r7,pc}
```
