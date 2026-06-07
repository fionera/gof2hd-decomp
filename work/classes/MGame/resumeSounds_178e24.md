# MGame::resumeSounds
elf 0x178e24 body 58
Sig: undefined __stdcall resumeSounds(void)

## decompile
```c

/* MGame::resumeSounds() */

void MGame::resumeSounds(void)

{
  uint *puVar1;
  uint uVar2;
  
  FModSound::resumeAll((FModSound *)**(undefined4 **)(DAT_00188e60 + 0x188e30));
  PlayerEgo::ResumeEngineSound();
  puVar1 = (uint *)Level::getEnemies();
  if (puVar1 != (uint *)0x0) {
    for (uVar2 = 0; uVar2 < *puVar1; uVar2 = uVar2 + 1) {
      KIPlayer::ResumeEngineSound(*(KIPlayer **)(puVar1[1] + uVar2 * 4));
    }
  }
  return;
}

```

## target disasm
```
  00188e24: push {r4,r5,r7,lr}
  00188e26: add r7,sp,#0x8
  00188e28: mov r4,r0
  00188e2a: ldr r0,[0x00188e60]
  00188e2c: add r0,pc
  00188e2e: ldr r0,[r0,#0x0]
  00188e30: ldr r0,[r0,#0x0]
  00188e32: blx 0x00071740
  00188e36: ldr r0,[r4,#0x58]
  00188e38: blx 0x000781a8
  00188e3c: ldr r0,[r4,#0x78]
  00188e3e: blx 0x00071eb4
  00188e42: mov r4,r0
  00188e44: cbz r0,0x00188e5c
  00188e46: movs r5,#0x0
  00188e48: b 0x00188e56
  00188e4a: ldr r0,[r4,#0x4]
  00188e4c: ldr.w r0,[r0,r5,lsl #0x2]
  00188e50: blx 0x000781b4
  00188e54: adds r5,#0x1
  00188e56: ldr r0,[r4,#0x0]
  00188e58: cmp r5,r0
  00188e5a: bcc 0x00188e4a
  00188e5c: pop {r4,r5,r7,pc}
```
