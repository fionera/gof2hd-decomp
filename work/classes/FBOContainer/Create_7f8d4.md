# FBOContainer::Create
elf 0x7f8d4 body 292
Sig: undefined __stdcall Create(int param_1, int param_2, bool param_3, bool param_4)

## decompile
```c

/* AbyssEngine::FBOContainer::Create(int, int, bool, bool) */

void AbyssEngine::FBOContainer::Create(int param_1,int param_2,bool param_3,bool param_4)

{
  undefined4 uVar1;
  
  *(int *)(param_1 + 0xc) = param_2;
  *(uint *)(param_1 + 0x10) = (uint)param_3;
  glGenFramebuffers(1,param_1);
  glBindFramebuffer(0x8d40,*(undefined4 *)param_1);
  glGenTextures(1,param_1 + 4);
  glBindTexture(0xde1,*(undefined4 *)(param_1 + 4));
  glPixelStorei(0xcf5,1);
  glTexParameteri(0xde1,0x2802,0x812f);
  glTexParameteri(0xde1,0x2803,0x812f);
  if (param_4) {
    glTexParameteri(0xde1,0x2800,0x2601);
    uVar1 = 0x2601;
  }
  else {
    glTexParameteri(0xde1,0x2800,0x2600);
    uVar1 = 0x2600;
  }
  glTexParameteri(0xde1,0x2801,uVar1);
  glTexImage2D(0xde1,0,0x1908,*(undefined4 *)(param_1 + 0xc),*(undefined4 *)(param_1 + 0x10),0,
               0x1908,0x1401,0);
  glFramebufferTexture2D(0x8d40,0x8ce0,0xde1,*(undefined4 *)(param_1 + 4),0);
  glGenRenderbuffers(1,param_1 + 8);
  glBindRenderbuffer(0x8d41,*(undefined4 *)(param_1 + 8));
  glRenderbufferStorage
            (0x8d41,0x81a5,*(undefined4 *)(param_1 + 0xc),*(undefined4 *)(param_1 + 0x10));
  glFramebufferRenderbuffer(0x8d40,0x8d00,0x8d41,*(undefined4 *)(param_1 + 8));
  glCheckFramebufferStatus(0x8d40);
  *(undefined1 *)(param_1 + 0x18) = 1;
  (*(code *)(DAT_001ab474 + 0x1ab478))(0x8d40,*(undefined4 *)(*(int *)(param_1 + 0x14) + 0x40c));
  return;
}

```

## target disasm
```
  0008f8d4: push {r4,r5,r7,lr}
  0008f8d6: add r7,sp,#0x8
  0008f8d8: sub sp,#0x18
  0008f8da: mov r4,r0
  0008f8dc: strd r1,r2,[r0,#0xc]
  0008f8e0: movs r0,#0x1
  0008f8e2: mov r1,r4
  0008f8e4: mov r5,r3
  0008f8e6: blx 0x00070030
  0008f8ea: ldr r1,[r4,#0x0]
  0008f8ec: movw r0,#0x8d40
  0008f8f0: blx 0x0006ee6c
  0008f8f4: adds r1,r4,#0x4
  0008f8f6: movs r0,#0x1
  0008f8f8: blx 0x0006f238
  0008f8fc: ldr r1,[r4,#0x4]
  0008f8fe: movw r0,#0xde1
  0008f902: blx 0x0006f244
  0008f906: movw r0,#0xcf5
  0008f90a: movs r1,#0x1
  0008f90c: blx 0x0006f25c
  0008f910: movw r0,#0xde1
  0008f914: movw r1,#0x2802
  0008f918: movw r2,#0x812f
  0008f91c: blx 0x0006f250
  0008f920: movw r0,#0xde1
  0008f924: movw r1,#0x2803
  0008f928: movw r2,#0x812f
  0008f92c: blx 0x0006f250
  0008f930: movw r0,#0xde1
  0008f934: mov.w r1,#0x2800
  0008f938: cbz r5,0x0008f950
  0008f93a: movw r2,#0x2601
  0008f93e: blx 0x0006f250
  0008f942: movw r0,#0xde1
  0008f946: movw r1,#0x2801
  0008f94a: movw r2,#0x2601
  0008f94e: b 0x0008f964
  0008f950: mov.w r2,#0x2600
  0008f954: blx 0x0006f250
  0008f958: movw r0,#0xde1
  0008f95c: movw r1,#0x2801
  0008f960: mov.w r2,#0x2600
  0008f964: blx 0x0006f250
  0008f968: ldrd r3,r0,[r4,#0xc]
  0008f96c: movs r5,#0x0
  0008f96e: movw r1,#0x1401
  0008f972: movw r2,#0x1908
  0008f976: strd r0,r5,[sp,#0x0]
  0008f97a: movw r0,#0xde1
  0008f97e: strd r2,r1,[sp,#0x8]
  0008f982: movs r1,#0x0
  0008f984: movw r2,#0x1908
  0008f988: str r5,[sp,#0x10]
  0008f98a: blx 0x0006f28c
  0008f98e: ldr r3,[r4,#0x4]
  0008f990: movw r0,#0x8d40
  0008f994: movw r1,#0x8ce0
  0008f998: movw r2,#0xde1
  0008f99c: str r5,[sp,#0x0]
  0008f99e: blx 0x0007003c
  0008f9a2: add.w r1,r4,#0x8
  0008f9a6: movs r0,#0x1
  0008f9a8: movs r5,#0x1
  0008f9aa: blx 0x00070048
  0008f9ae: ldr r1,[r4,#0x8]
  0008f9b0: movw r0,#0x8d41
  0008f9b4: blx 0x00070054
  0008f9b8: ldrd r2,r3,[r4,#0xc]
  0008f9bc: movw r0,#0x8d41
  0008f9c0: movw r1,#0x81a5
  0008f9c4: blx 0x00070060
  0008f9c8: ldr r3,[r4,#0x8]
  0008f9ca: movw r0,#0x8d40
  0008f9ce: mov.w r1,#0x8d00
  0008f9d2: movw r2,#0x8d41
  0008f9d6: blx 0x0007006c
  0008f9da: movw r0,#0x8d40
  0008f9de: blx 0x00070078
  0008f9e2: ldr r0,[r4,#0x14]
  0008f9e4: strb r5,[r4,#0x18]
  0008f9e6: ldr.w r1,[r0,#0x40c]
  0008f9ea: movw r0,#0x8d40
  0008f9ee: add sp,#0x18
  0008f9f0: pop.w {r4,r5,r7,lr}
  0008f9f4: b.w 0x001ab468
```
