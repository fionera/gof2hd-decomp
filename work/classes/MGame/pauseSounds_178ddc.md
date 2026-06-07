# MGame::pauseSounds
elf 0x178ddc body 66
Sig: undefined __stdcall pauseSounds(void)

## decompile
```c

/* MGame::pauseSounds() */

void MGame::pauseSounds(void)

{
  int in_r0;
  undefined4 *puVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar1 = (undefined4 *)(DAT_00188e20 + 0x188dec);
  *(undefined1 *)(in_r0 + 0x1a6) = *(undefined1 *)(in_r0 + 0x5d);
  FModSound::pauseAllPlayingSoundFXEvents(*(FModSound **)*puVar1);
  PlayerEgo::PauseEngineSound();
  puVar2 = (uint *)Level::getEnemies();
  if (puVar2 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      KIPlayer::PauseEngineSound(*(KIPlayer **)(puVar2[1] + uVar3 * 4));
    }
  }
  return;
}

```

## target disasm
```
  00188ddc: push {r4,r5,r7,lr}
  00188dde: add r7,sp,#0x8
  00188de0: mov r4,r0
  00188de2: ldr r0,[0x00188e20]
  00188de4: ldrb.w r1,[r4,#0x5d]
  00188de8: add r0,pc
  00188dea: strb.w r1,[r4,#0x1a6]
  00188dee: ldr r0,[r0,#0x0]
  00188df0: ldr r0,[r0,#0x0]
  00188df2: blx 0x00078184
  00188df6: ldr r0,[r4,#0x58]
  00188df8: blx 0x00078190
  00188dfc: ldr r0,[r4,#0x78]
  00188dfe: blx 0x00071eb4
  00188e02: mov r4,r0
  00188e04: cbz r0,0x00188e1c
  00188e06: movs r5,#0x0
  00188e08: b 0x00188e16
  00188e0a: ldr r0,[r4,#0x4]
  00188e0c: ldr.w r0,[r0,r5,lsl #0x2]
  00188e10: blx 0x0007819c
  00188e14: adds r5,#0x1
  00188e16: ldr r0,[r4,#0x0]
  00188e18: cmp r5,r0
  00188e1a: bcc 0x00188e0a
  00188e1c: pop {r4,r5,r7,pc}
```
