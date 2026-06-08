# AbyssEngine::MeshDraw
elf 0x6d3ac body 350
Sig: undefined __stdcall MeshDraw(Engine * param_1, Mesh * param_2)

## decompile
```c

/* AbyssEngine::MeshDraw(AbyssEngine::Engine*, AbyssEngine::Mesh*) */

undefined4 AbyssEngine::MeshDraw(Engine *param_1,Mesh *param_2)

{
  int iVar1;
  
  if (((param_2 != (Mesh *)0x0) && (*(short *)(param_2 + 2) != 0)) && (((byte)*param_2 & 1) != 0)) {
    if ((**(char **)(DAT_0007d50c + 0x7d3cc) == '\0') && (param_2[0x5c] != (Mesh)0x0)) {
      glBindBuffer(0x8892,*(undefined4 *)(param_2 + 0x60));
      Engine::AEClientState(param_1,0x8074,true);
      glVertexPointer(3,0x1406,0,0);
      glBindBuffer(0x8893,*(undefined4 *)(param_2 + 100));
      if ((int)((uint)(byte)*param_2 << 0x1e) < 0) {
        glBindBuffer(0x8892,*(undefined4 *)(param_2 + 0x68));
        Engine::AEClientState(param_1,0x8078,true);
        glTexCoordPointer(2,0x1406,0,0);
      }
      else {
        Engine::AEClientState(param_1,0x8078,false);
      }
      if ((int)((uint)(byte)*param_2 << 0x1d) < 0) {
        glBindBuffer(0x8892,*(undefined4 *)(param_2 + 0x6c));
        Engine::AEClientState(param_1,0x8075,true);
        glNormalPointer(0x1406,0,0);
      }
      else {
        Engine::AEClientState(param_1,0x8075,false);
      }
      if ((int)((uint)(byte)*param_2 << 0x1c) < 0) {
        glBindBuffer(0x8892,*(undefined4 *)(param_2 + 0x78));
        Engine::AEClientState(param_1,0x8076,true);
        glColorPointer(4,0x1406,0,0);
      }
      else {
        Engine::AEClientState(param_1,0x8076,false);
      }
      glDrawElements(4,*(undefined2 *)(param_2 + 0x28),0x1403,0);
      if (param_1[0xfe] != (Engine)0x0) {
        iVar1 = __aeabi_uidiv(*(undefined2 *)(param_2 + 0x28),3);
        if (param_1[0xfd] == (Engine)0x0) {
          *(int *)(param_1 + 100) = *(int *)(param_1 + 100) + 1;
          *(int *)(param_1 + 0x68) = iVar1 + *(int *)(param_1 + 0x68);
        }
        else {
          *(int *)(param_1 + 0x6c) = iVar1 + *(int *)(param_1 + 0x6c);
          *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 1;
        }
      }
      glBindBuffer(0x8892,0);
      glBindBuffer(0x8893,0);
    }
    else {
      Engine::RenderMesh((Mesh *)param_1);
    }
    return 1;
  }
  return 0xfffffffc;
}

```

## target disasm
```
  0007d3ac: push {r4,r5,r7,lr}
  0007d3ae: add r7,sp,#0x8
  0007d3b0: mov r5,r1
  0007d3b2: mov r4,r0
  0007d3b4: cbz r1,0x0007d3c0
  0007d3b6: ldrh r0,[r5,#0x2]
  0007d3b8: cbz r0,0x0007d3c0
  0007d3ba: ldrb r0,[r5,#0x0]
  0007d3bc: lsls r0,r0,#0x1f
  0007d3be: bne 0x0007d3c6
  0007d3c0: mvn r0,#0x3
  0007d3c4: pop {r4,r5,r7,pc}
  0007d3c6: ldr r0,[0x0007d50c]
  0007d3c8: add r0,pc
  0007d3ca: ldr r0,[r0,#0x0]
  0007d3cc: ldrb r0,[r0,#0x0]
  0007d3ce: cbz r0,0x0007d3da
  0007d3d0: mov r0,r4
  0007d3d2: mov r1,r5
  0007d3d4: blx 0x0006ecf8
  0007d3d8: b 0x0007d506
  0007d3da: ldrb.w r0,[r5,#0x5c]
  0007d3de: cmp r0,#0x0
  0007d3e0: beq 0x0007d3d0
  0007d3e2: ldr r1,[r5,#0x60]
  0007d3e4: movw r0,#0x8892
  0007d3e8: blx 0x0006ed04
  0007d3ec: mov r0,r4
  0007d3ee: movw r1,#0x8074
  0007d3f2: movs r2,#0x1
  0007d3f4: blx 0x0006ed10
  0007d3f8: movs r0,#0x3
  0007d3fa: movw r1,#0x1406
  0007d3fe: movs r2,#0x0
  0007d400: movs r3,#0x0
  0007d402: blx 0x0006ed1c
  0007d406: ldr r1,[r5,#0x64]
  0007d408: movw r0,#0x8893
  0007d40c: blx 0x0006ed04
  0007d410: ldrb r0,[r5,#0x0]
  0007d412: lsls r0,r0,#0x1e
  0007d414: bmi 0x0007d424
  0007d416: mov r0,r4
  0007d418: movw r1,#0x8078
  0007d41c: movs r2,#0x0
  0007d41e: blx 0x0006ed10
  0007d422: b 0x0007d448
  0007d424: ldr r1,[r5,#0x68]
  0007d426: movw r0,#0x8892
  0007d42a: blx 0x0006ed04
  0007d42e: mov r0,r4
  0007d430: movw r1,#0x8078
  0007d434: movs r2,#0x1
  0007d436: blx 0x0006ed10
  0007d43a: movs r0,#0x2
  0007d43c: movw r1,#0x1406
  0007d440: movs r2,#0x0
  0007d442: movs r3,#0x0
  0007d444: blx 0x0006ed28
  0007d448: ldrb r0,[r5,#0x0]
  0007d44a: lsls r0,r0,#0x1d
  0007d44c: bmi 0x0007d45c
  0007d44e: mov r0,r4
  0007d450: movw r1,#0x8075
  0007d454: movs r2,#0x0
  0007d456: blx 0x0006ed10
  0007d45a: b 0x0007d47e
  0007d45c: ldr r1,[r5,#0x6c]
  0007d45e: movw r0,#0x8892
  0007d462: blx 0x0006ed04
  0007d466: mov r0,r4
  0007d468: movw r1,#0x8075
  0007d46c: movs r2,#0x1
  0007d46e: blx 0x0006ed10
  0007d472: movw r0,#0x1406
  0007d476: movs r1,#0x0
  0007d478: movs r2,#0x0
  0007d47a: blx 0x0006ed34
  0007d47e: ldrb r0,[r5,#0x0]
  0007d480: lsls r0,r0,#0x1c
  0007d482: bmi 0x0007d492
  0007d484: mov r0,r4
  0007d486: movw r1,#0x8076
  0007d48a: movs r2,#0x0
  0007d48c: blx 0x0006ed10
  0007d490: b 0x0007d4b6
  0007d492: ldr r1,[r5,#0x78]
  0007d494: movw r0,#0x8892
  0007d498: blx 0x0006ed04
  0007d49c: mov r0,r4
  0007d49e: movw r1,#0x8076
  0007d4a2: movs r2,#0x1
  0007d4a4: blx 0x0006ed10
  0007d4a8: movs r0,#0x4
  0007d4aa: movw r1,#0x1406
  0007d4ae: movs r2,#0x0
  0007d4b0: movs r3,#0x0
  0007d4b2: blx 0x0006ed40
  0007d4b6: ldrh r1,[r5,#0x28]
  0007d4b8: movs r0,#0x4
  0007d4ba: movw r2,#0x1403
  0007d4be: movs r3,#0x0
  0007d4c0: blx 0x0006ed4c
  0007d4c4: ldrb.w r0,[r4,#0xfe]
  0007d4c8: cbz r0,0x0007d4f2
  0007d4ca: ldrh r0,[r5,#0x28]
  0007d4cc: movs r1,#0x3
  0007d4ce: blx 0x0006ec2c
  0007d4d2: ldrb.w r1,[r4,#0xfd]
  0007d4d6: cbz r1,0x0007d4e6
  0007d4d8: ldr r2,[r4,#0x6c]
  0007d4da: ldr r1,[r4,#0x60]
  0007d4dc: add r0,r2
  0007d4de: str r0,[r4,#0x6c]
  0007d4e0: adds r0,r1,#0x1
  0007d4e2: str r0,[r4,#0x60]
  0007d4e4: b 0x0007d4f2
  0007d4e6: ldrd r1,r2,[r4,#0x64]
  0007d4ea: add r0,r2
  0007d4ec: adds r1,#0x1
  0007d4ee: strd r1,r0,[r4,#0x64]
  0007d4f2: movw r0,#0x8892
  0007d4f6: movs r1,#0x0
  0007d4f8: blx 0x0006ed04
  0007d4fc: movw r0,#0x8893
  0007d500: movs r1,#0x0
  0007d502: blx 0x0006ed04
  0007d506: movs r0,#0x1
  0007d508: pop {r4,r5,r7,pc}
```
