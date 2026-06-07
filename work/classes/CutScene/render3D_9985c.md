# CutScene::render3D
elf 0x9985c body 108
Sig: undefined __stdcall render3D(void)

## decompile
```c

/* CutScene::render3D() */

void CutScene::render3D(void)

{
  uint *in_r0;
  uint uVar1;
  uint *puVar2;
  
  if (*in_r0 != 0) {
    uVar1 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
    in_r0[0x16] = uVar1;
    Level::update((ulonglong)*in_r0,SUB41(uVar1,0));
    Level::render(*in_r0);
  }
  if (in_r0[10] != 0) {
    AEGeometry::render();
  }
  if (in_r0[0xb] != 0) {
    AEGeometry::render();
  }
  if (in_r0[0xc] != 0) {
    AEGeometry::render();
  }
  if (in_r0[0xd] != 0) {
    AEGeometry::render();
  }
  puVar2 = (uint *)in_r0[0xe];
  if (puVar2 != (uint *)0x0) {
    for (uVar1 = 0; uVar1 < *puVar2; uVar1 = uVar1 + 1) {
      AEGeometry::render();
      puVar2 = (uint *)in_r0[0xe];
    }
  }
  return;
}

```

## target disasm
```
  000a985c: push {r2,r3,r4,r5,r7,lr}
  000a985e: add r7,sp,#0x10
  000a9860: mov r4,r0
  000a9862: ldr r0,[r0,#0x0]
  000a9864: cbz r0,0x000a988a
  000a9866: ldr r0,[0x000a98c8]
  000a9868: add r0,pc
  000a986a: ldr r0,[r0,#0x0]
  000a986c: ldr r0,[r0,#0x0]
  000a986e: blx 0x00070e4c
  000a9872: mov r2,r0
  000a9874: ldr r0,[r4,#0x0]
  000a9876: asrs r3,r2,#0x1f
  000a9878: movs r1,#0x0
  000a987a: str r2,[r4,#0x58]
  000a987c: str r1,[sp,#0x0]
  000a987e: blx 0x00072220
  000a9882: ldr r0,[r4,#0x0]
  000a9884: ldr r1,[r4,#0x58]
  000a9886: blx 0x0007222c
  000a988a: ldr r0,[r4,#0x28]
  000a988c: cbz r0,0x000a9892
  000a988e: blx 0x00072238
  000a9892: ldr r0,[r4,#0x2c]
  000a9894: cbz r0,0x000a989a
  000a9896: blx 0x00072238
  000a989a: ldr r0,[r4,#0x30]
  000a989c: cbz r0,0x000a98a2
  000a989e: blx 0x00072238
  000a98a2: ldr r0,[r4,#0x34]
  000a98a4: cbz r0,0x000a98aa
  000a98a6: blx 0x00072238
  000a98aa: ldr r0,[r4,#0x38]
  000a98ac: cbz r0,0x000a98c6
  000a98ae: movs r5,#0x0
  000a98b0: b 0x000a98c0
  000a98b2: ldr r0,[r0,#0x4]
  000a98b4: ldr.w r0,[r0,r5,lsl #0x2]
  000a98b8: blx 0x00072238
  000a98bc: ldr r0,[r4,#0x38]
  000a98be: adds r5,#0x1
  000a98c0: ldr r1,[r0,#0x0]
  000a98c2: cmp r5,r1
  000a98c4: bcc 0x000a98b2
  000a98c6: pop {r2,r3,r4,r5,r7,pc}
```
