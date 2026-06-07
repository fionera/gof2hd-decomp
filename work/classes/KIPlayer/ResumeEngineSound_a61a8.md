# KIPlayer::ResumeEngineSound
elf 0xa61a8 body 24
Sig: undefined __thiscall ResumeEngineSound(KIPlayer * this)

## decompile
```c

/* KIPlayer::ResumeEngineSound() */

void __thiscall KIPlayer::ResumeEngineSound(KIPlayer *this)

{
  if ((*(int *)(this + 4) != 0) && (*(int *)(this + 0xf8) != -1)) {
    (*(code *)(DAT_001aba34 + 0x1aba38))(*(int *)(this + 4),0);
    return;
  }
  return;
}

```

## target disasm
```
  000b61a8: mov r1,r0
  000b61aa: ldr r0,[r0,#0x4]
  000b61ac: cbz r0,0x000b61bc
  000b61ae: ldr.w r1,[r1,#0xf8]
  000b61b2: adds r1,#0x1
  000b61b4: itt ne
  000b61b6: mov.ne r1,#0x0
  000b61b8: b.ne.w 0x001aba28
  000b61bc: bx lr
  001aba28: bx pc
```
