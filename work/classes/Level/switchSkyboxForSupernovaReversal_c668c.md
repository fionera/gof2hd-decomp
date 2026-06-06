# Level::switchSkyboxForSupernovaReversal
elf 0xc668c body 98
Sig: undefined __thiscall switchSkyboxForSupernovaReversal(Level * this)

## decompile
```c

/* Level::switchSkyboxForSupernovaReversal() */

void __thiscall Level::switchSkyboxForSupernovaReversal(Level *this)

{
  short sVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  PaintCanvas *this_00;
  
  puVar3 = *(undefined4 **)(DAT_000d66f0 + 0xd669e);
  this_00 = (PaintCanvas *)*puVar3;
  Status::getSystem();
  sVar1 = SolarSystem::getTextureIndex();
  AbyssEngine::PaintCanvas::MeshCreate(this_00,sVar1 + 0x4588,(uint *)(this + 4),false);
  uVar4 = *puVar3;
  Status::getSystem();
  iVar2 = SolarSystem::getTextureIndex();
  AbyssEngine::PaintCanvas::TextureCreate
            ((ushort)uVar4,(uint *)(iVar2 + 0x2751U & 0xffff),(bool)((char)this + -0x68));
  *(undefined4 *)(this + 0xc) = 0xffffffff;
  return;
}

```
## target disasm
```
  000d668c: push {r4,r5,r6,r7,lr}
  000d668e: add r7,sp,#0xc
  000d6690: push.w r8
  000d6694: mov r4,r0
  000d6696: ldr r0,[0x000d66f0]
  000d6698: ldr r1,[0x000d66f4]
  000d669a: add r0,pc
  000d669c: add r1,pc
  000d669e: ldr r6,[r0,#0x0]
  000d66a0: ldr r5,[r1,#0x0]
  000d66a2: ldr.w r8,[r6,#0x0]
  000d66a6: ldr r0,[r5,#0x0]
  000d66a8: blx 0x00071a10
  000d66ac: blx 0x0007390c
  000d66b0: movw r1,#0x4588
  000d66b4: add r0,r1
  000d66b6: adds r2,r4,#0x4
  000d66b8: movs r3,#0x0
  000d66ba: uxth r1,r0
  000d66bc: mov r0,r8
  000d66be: blx 0x0006fcf4
  000d66c2: ldr r0,[r5,#0x0]
  000d66c4: ldr r6,[r6,#0x0]
  000d66c6: blx 0x00071a10
  000d66ca: blx 0x0007390c
  000d66ce: movw r1,#0x2751
  000d66d2: add r0,r1
  000d66d4: add.w r2,r4,#0x198
  000d66d8: movs r3,#0x0
  000d66da: uxth r1,r0
  000d66dc: mov r0,r6
  000d66de: blx 0x0006fbb0
  000d66e2: mov.w r0,#0xffffffff
  000d66e6: str r0,[r4,#0xc]
  000d66e8: pop.w r8
  000d66ec: pop {r4,r5,r6,r7,pc}
```
