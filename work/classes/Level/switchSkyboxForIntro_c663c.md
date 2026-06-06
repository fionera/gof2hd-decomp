# Level::switchSkyboxForIntro
elf 0xc663c body 76
Sig: undefined __thiscall switchSkyboxForIntro(Level * this)

## decompile
```c

/* Level::switchSkyboxForIntro() */

void __thiscall Level::switchSkyboxForIntro(Level *this)

{
  uint *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  
  puVar2 = *(undefined4 **)(DAT_000d6688 + 0xd664e);
  AbyssEngine::PaintCanvas::MeshCreate((PaintCanvas *)*puVar2,0x4591,(uint *)(this + 4),false);
  AbyssEngine::PaintCanvas::TextureCreate((ushort)*puVar2,(uint *)0x275a,(bool)((char)this + -0x68))
  ;
  puVar1 = *(uint **)(this + 0xfc);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      KIPlayer::setDead();
      puVar1 = *(uint **)(this + 0xfc);
    }
  }
  return;
}

```
## target disasm
```
  000d663c: push {r4,r5,r7,lr}
  000d663e: add r7,sp,#0x8
  000d6640: mov r4,r0
  000d6642: ldr r0,[0x000d6688]
  000d6644: adds r2,r4,#0x4
  000d6646: movw r1,#0x4591
  000d664a: add r0,pc
  000d664c: movs r3,#0x0
  000d664e: ldr r5,[r0,#0x0]
  000d6650: ldr r0,[r5,#0x0]
  000d6652: blx 0x0006fcf4
  000d6656: ldr r0,[r5,#0x0]
  000d6658: add.w r2,r4,#0x198
  000d665c: movw r1,#0x275a
  000d6660: movs r3,#0x0
  000d6662: blx 0x0006fbb0
  000d6666: ldr.w r0,[r4,#0xfc]
  000d666a: cbz r0,0x000d6686
  000d666c: movs r5,#0x0
  000d666e: b 0x000d6680
  000d6670: ldr r0,[r0,#0x4]
  000d6672: ldr.w r0,[r0,r5,lsl #0x2]
  000d6676: blx 0x0007408c
  000d667a: ldr.w r0,[r4,#0xfc]
  000d667e: adds r5,#0x1
  000d6680: ldr r1,[r0,#0x0]
  000d6682: cmp r5,r1
  000d6684: bcc 0x000d6670
  000d6686: pop {r4,r5,r7,pc}
```
