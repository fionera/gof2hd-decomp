# AbyssEngine::MeshReleaseIntern
elf 0x6c380 body 332
Sig: undefined __stdcall MeshReleaseIntern(Engine * param_1, Mesh * * param_2)

## decompile
```c

/* AbyssEngine::MeshReleaseIntern(AbyssEngine::Engine*, AbyssEngine::Mesh**) */

void AbyssEngine::MeshReleaseIntern(Engine *param_1,Mesh **param_2)

{
  Mesh MVar1;
  int iVar2;
  void *pvVar3;
  Mesh *pMVar4;
  
  pMVar4 = *param_2;
  if (pMVar4 != (Mesh *)0x0) {
    if (pMVar4[0x38] == (Mesh)0x0) {
      if (pMVar4[0x5c] != (Mesh)0x0) {
        glDeleteBuffers(1,pMVar4 + 0x60);
        glDeleteBuffers(1,*param_2 + 100);
        pMVar4 = *param_2;
        MVar1 = *pMVar4;
        if (((byte)MVar1 & 2) != 0) {
          glDeleteBuffers(1,pMVar4 + 0x68);
          pMVar4 = *param_2;
          MVar1 = *pMVar4;
        }
        if (((int)((uint)(byte)MVar1 << 0x1d) < 0) &&
           (glDeleteBuffers(1,pMVar4 + 0x6c), **(char **)(DAT_0007c4cc + 0x7c3de) != '\0')) {
          glDeleteBuffers(1,*param_2 + 0x70);
          glDeleteBuffers(1,*param_2 + 0x74);
        }
        pMVar4 = *param_2;
        if ((int)((uint)(byte)*pMVar4 << 0x1c) < 0) {
          glDeleteBuffers(1,pMVar4 + 0x78);
          pMVar4 = *param_2;
        }
      }
      pMVar4 = pMVar4 + 0x2c;
      if (*(void **)pMVar4 != (void *)0x0) {
        operator_delete__(*(void **)pMVar4);
        pMVar4 = *param_2 + 0x2c;
      }
      *(undefined4 *)pMVar4 = 0;
      pMVar4 = *param_2 + 4;
      if (*(void **)pMVar4 != (void *)0x0) {
        operator_delete__(*(void **)pMVar4);
        pMVar4 = *param_2 + 4;
      }
      *(undefined4 *)pMVar4 = 0;
      pMVar4 = *param_2 + 8;
      if (*(void **)pMVar4 != (void *)0x0) {
        operator_delete__(*(void **)pMVar4);
        pMVar4 = *param_2 + 8;
      }
      *(undefined4 *)pMVar4 = 0;
      pMVar4 = *param_2 + 0xc;
      if (*(void **)pMVar4 != (void *)0x0) {
        operator_delete__(*(void **)pMVar4);
        pMVar4 = *param_2 + 0xc;
      }
      *(undefined4 *)pMVar4 = 0;
      pMVar4 = *param_2 + 0x10;
      if (*(void **)pMVar4 != (void *)0x0) {
        operator_delete__(*(void **)pMVar4);
        pMVar4 = *param_2 + 0x10;
      }
      iVar2 = DAT_0007c4d0;
      *(undefined4 *)pMVar4 = 0;
      if (**(char **)(iVar2 + 0x7c478) != '\0') {
        pMVar4 = *param_2 + 0x14;
        if (*(void **)pMVar4 != (void *)0x0) {
          operator_delete__(*(void **)pMVar4);
          pMVar4 = *param_2 + 0x14;
        }
        *(undefined4 *)pMVar4 = 0;
        pMVar4 = *param_2 + 0x18;
        if (*(void **)pMVar4 != (void *)0x0) {
          operator_delete__(*(void **)pMVar4);
          pMVar4 = *param_2 + 0x18;
        }
        *(undefined4 *)pMVar4 = 0;
      }
    }
    pMVar4 = *param_2 + 0x34;
    if (*(Transform **)pMVar4 != (Transform *)0x0) {
      pvVar3 = (void *)Transform::~Transform(*(Transform **)pMVar4);
      operator_delete(pvVar3);
      pMVar4 = *param_2 + 0x34;
    }
    *(undefined4 *)pMVar4 = 0;
    if (*param_2 != (Mesh *)0x0) {
      operator_delete(*param_2);
    }
    *param_2 = (Mesh *)0x0;
  }
  return;
}

```

## target disasm
```
  0007c380: push {r4,r5,r7,lr}
  0007c382: add r7,sp,#0x8
  0007c384: mov r4,r1
  0007c386: ldr r1,[r1,#0x0]
  0007c388: cmp r1,#0x0
  0007c38a: beq.w 0x0007c4ca
  0007c38e: ldrb.w r0,[r1,#0x38]
  0007c392: cmp r0,#0x0
  0007c394: bne.w 0x0007c4a6
  0007c398: ldrb.w r0,[r1,#0x5c]
  0007c39c: cbz r0,0x0007c40c
  0007c39e: adds r1,#0x60
  0007c3a0: movs r0,#0x1
  0007c3a2: blx 0x0006ebf0
  0007c3a6: ldr r0,[r4,#0x0]
  0007c3a8: add.w r1,r0,#0x64
  0007c3ac: movs r0,#0x1
  0007c3ae: blx 0x0006ebf0
  0007c3b2: ldr r0,[r4,#0x0]
  0007c3b4: ldrb r1,[r0,#0x0]
  0007c3b6: tst r1,#0x2
  0007c3ba: beq 0x0007c3ca
  0007c3bc: add.w r1,r0,#0x68
  0007c3c0: movs r0,#0x1
  0007c3c2: blx 0x0006ebf0
  0007c3c6: ldr r0,[r4,#0x0]
  0007c3c8: ldrb r1,[r0,#0x0]
  0007c3ca: lsls r1,r1,#0x1d
  0007c3cc: bpl 0x0007c3fa
  0007c3ce: add.w r1,r0,#0x6c
  0007c3d2: movs r0,#0x1
  0007c3d4: blx 0x0006ebf0
  0007c3d8: ldr r0,[0x0007c4cc]
  0007c3da: add r0,pc
  0007c3dc: ldr r0,[r0,#0x0]
  0007c3de: ldrb r0,[r0,#0x0]
  0007c3e0: cbz r0,0x0007c3fa
  0007c3e2: ldr r0,[r4,#0x0]
  0007c3e4: add.w r1,r0,#0x70
  0007c3e8: movs r0,#0x1
  0007c3ea: blx 0x0006ebf0
  0007c3ee: ldr r0,[r4,#0x0]
  0007c3f0: add.w r1,r0,#0x74
  0007c3f4: movs r0,#0x1
  0007c3f6: blx 0x0006ebf0
  0007c3fa: ldr r1,[r4,#0x0]
  0007c3fc: ldrb r0,[r1,#0x0]
  0007c3fe: lsls r0,r0,#0x1c
  0007c400: bpl 0x0007c40c
  0007c402: adds r1,#0x78
  0007c404: movs r0,#0x1
  0007c406: blx 0x0006ebf0
  0007c40a: ldr r1,[r4,#0x0]
  0007c40c: ldr.w r0,[r1,#0x2c]!
  0007c410: cbz r0,0x0007c41c
  0007c412: blx 0x0006ebfc
  0007c416: ldr r0,[r4,#0x0]
  0007c418: add.w r1,r0,#0x2c
  0007c41c: movs r5,#0x0
  0007c41e: str r5,[r1,#0x0]
  0007c420: ldr r1,[r4,#0x0]
  0007c422: ldr.w r0,[r1,#0x4]!
  0007c426: cbz r0,0x0007c430
  0007c428: blx 0x0006ebfc
  0007c42c: ldr r0,[r4,#0x0]
  0007c42e: adds r1,r0,#0x4
  0007c430: str r5,[r1,#0x0]
  0007c432: ldr r1,[r4,#0x0]
  0007c434: ldr.w r0,[r1,#0x8]!
  0007c438: cbz r0,0x0007c444
  0007c43a: blx 0x0006ebfc
  0007c43e: ldr r0,[r4,#0x0]
  0007c440: add.w r1,r0,#0x8
  0007c444: movs r5,#0x0
  0007c446: str r5,[r1,#0x0]
  0007c448: ldr r1,[r4,#0x0]
  0007c44a: ldr.w r0,[r1,#0xc]!
  0007c44e: cbz r0,0x0007c45a
  0007c450: blx 0x0006ebfc
  0007c454: ldr r0,[r4,#0x0]
  0007c456: add.w r1,r0,#0xc
  0007c45a: str r5,[r1,#0x0]
  0007c45c: ldr r1,[r4,#0x0]
  0007c45e: ldr.w r0,[r1,#0x10]!
  0007c462: cbz r0,0x0007c46e
  0007c464: blx 0x0006ebfc
  0007c468: ldr r0,[r4,#0x0]
  0007c46a: add.w r1,r0,#0x10
  0007c46e: ldr r0,[0x0007c4d0]
  0007c470: movs r2,#0x0
  0007c472: str r2,[r1,#0x0]
  0007c474: add r0,pc
  0007c476: ldr r0,[r0,#0x0]
  0007c478: ldrb r0,[r0,#0x0]
  0007c47a: cbz r0,0x0007c4a6
  0007c47c: ldr r1,[r4,#0x0]
  0007c47e: ldr.w r0,[r1,#0x14]!
  0007c482: cbz r0,0x0007c48e
  0007c484: blx 0x0006ebfc
  0007c488: ldr r0,[r4,#0x0]
  0007c48a: add.w r1,r0,#0x14
  0007c48e: movs r5,#0x0
  0007c490: str r5,[r1,#0x0]
  0007c492: ldr r1,[r4,#0x0]
  0007c494: ldr.w r0,[r1,#0x18]!
  0007c498: cbz r0,0x0007c4a4
  0007c49a: blx 0x0006ebfc
  0007c49e: ldr r0,[r4,#0x0]
  0007c4a0: add.w r1,r0,#0x18
  0007c4a4: str r5,[r1,#0x0]
  0007c4a6: ldr r1,[r4,#0x0]
  0007c4a8: ldr.w r0,[r1,#0x34]!
  0007c4ac: cbz r0,0x0007c4bc
  0007c4ae: blx 0x0006eba8
  0007c4b2: blx 0x0006eb48
  0007c4b6: ldr r0,[r4,#0x0]
  0007c4b8: add.w r1,r0,#0x34
  0007c4bc: movs r5,#0x0
  0007c4be: str r5,[r1,#0x0]
  0007c4c0: ldr r0,[r4,#0x0]
  0007c4c2: cbz r0,0x0007c4c8
  0007c4c4: blx 0x0006eb48
  0007c4c8: str r5,[r4,#0x0]
  0007c4ca: pop {r4,r5,r7,pc}
```
