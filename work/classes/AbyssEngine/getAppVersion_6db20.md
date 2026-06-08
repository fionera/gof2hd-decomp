# AbyssEngine::getAppVersion
elf 0x6db20 body 360
Sig: undefined __stdcall getAppVersion(void)

## decompile
```c

/* AbyssEngine::Engine::InitGL(bool, int, int)
   AbyssEngine::getAppVersion() */

void AbyssEngine::getAppVersion(void)

{
  int iVar1;
  Engine *in_r0;
  int iVar2;
  FileInterfaceAndroid *this;
  FBOContainer *pFVar3;
  char in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  int *piVar4;
  char *pcVar5;
  String aSStack_3c [12];
  
  piVar4 = *(int **)(DAT_0007dca8 + 0x7db34);
  iVar2 = *piVar4;
  *(undefined4 *)(in_r0 + 0x418) = 0;
  *(undefined4 *)(in_r0 + 0x368) = in_r2;
  *(undefined4 *)(in_r0 + 0x36c) = in_r3;
  *(undefined4 *)(in_r0 + 0x370) = in_r2;
  *(undefined4 *)(in_r0 + 0x374) = in_r3;
  this = operator_new(0x38);
  FileInterfaceAndroid::FileInterfaceAndroid(this);
  *(FileInterfaceAndroid **)(in_r0 + 0x24) = this;
  AEFile::SetInterface((FileInterface *)this);
  iVar1 = DAT_0007dcac;
  *(undefined4 *)(in_r0 + 0x10) = 0;
  in_r0[0x2c] = (Engine)0x0;
  in_r0[0x480] = (Engine)0x0;
  pcVar5 = *(char **)(iVar1 + 0x7db64);
  *pcVar5 = in_r1;
  *(undefined4 *)(in_r0 + 0x40c) = 0;
  Engine::ResetLightParam(in_r0);
  glViewport(0,0,*(undefined4 *)(in_r0 + 0x374),*(undefined4 *)(in_r0 + 0x370));
  if (*pcVar5 == '\0') {
    glEnable(0x803a);
    glDisable(0xb50);
    glLineWidth(0x3f800000);
  }
  else {
    Engine::ShaderInit(in_r0);
  }
  AEMath::Vector::operator=((Vector *)(in_r0 + 0x468),(Vector *)&stack0xffffffd0);
  *(undefined4 *)(in_r0 + 0x378) = 0;
  AEMath::Vector::operator=((Vector *)(in_r0 + 0x474),(Vector *)&stack0xffffffd0);
  *(undefined4 *)(in_r0 + 0x37c) = 0;
  glEnable(0xb71);
  Engine::GlEnable((uint)in_r0,true);
  glDisable(0xbe2);
  glCullFace(0x405);
  glEnable(0xb44);
  Engine::AfterGLInit(in_r0);
  PaintCanvas::Initialize((PaintCanvas *)**(undefined4 **)(in_r0 + 0x30),false);
  *(undefined4 *)(in_r0 + 0xc) = 0;
  glGetIntegerv(0xd33,in_r0 + 0xc);
  if ((*pcVar5 != '\0') && (**(char **)(DAT_0007dcb0 + 0x7dc32) != '\0')) {
    pFVar3 = operator_new(0x38);
    String::String(aSStack_3c,(char *)(DAT_0007dcb4 + 0x7dc44),false);
    FBOContainer::FBOContainer(pFVar3);
    *(FBOContainer **)(in_r0 + 0x418) = pFVar3;
    String::~String(aSStack_3c);
    FBOContainer::Create
              (*(int *)(in_r0 + 0x418),*(int *)(in_r0 + 0x368),
               SUB41(*(undefined4 *)(in_r0 + 0x36c),0),true);
  }
  iVar2 = *piVar4 - iVar2;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  0007db20: push {r4,r5,r6,r7,lr}
  0007db22: add r7,sp,#0xc
  0007db24: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10}
  0007db28: mov r4,r0
  0007db2a: ldr r0,[0x0007dca8]
  0007db2c: movs r5,#0x0
  0007db2e: mov r9,r1
  0007db30: add r0,pc
  0007db32: ldr.w r8,[r0,#0x0]
  0007db36: ldr.w r0,[r8,#0x0]
  0007db3a: str r0,[sp,#0x1c]
  0007db3c: movs r0,#0x38
  0007db3e: str.w r5,[r4,#0x418]
  0007db42: strd r2,r3,[r4,#0x368]
  0007db46: strd r2,r3,[r4,#0x370]
  0007db4a: blx 0x0006eb24
  0007db4e: mov r6,r0
  0007db50: blx 0x0006ed7c
  0007db54: mov r0,r6
  0007db56: str r6,[r4,#0x24]
  0007db58: blx 0x0006ed88
  0007db5c: ldr r0,[0x0007dcac]
  0007db5e: str r5,[r4,#0x10]
  0007db60: add r0,pc
  0007db62: strb.w r5,[r4,#0x2c]
  0007db66: strb.w r5,[r4,#0x480]
  0007db6a: ldr.w r10,[r0,#0x0]
  0007db6e: mov r0,r4
  0007db70: strb.w r9,[r10,#0x0]
  0007db74: str.w r5,[r4,#0x40c]
  0007db78: blx 0x0006ed94
  0007db7c: ldrd r3,r2,[r4,#0x370]
  0007db80: movs r0,#0x0
  0007db82: movs r1,#0x0
  0007db84: blx 0x0006eda0
  0007db88: ldrb.w r0,[r10,#0x0]
  0007db8c: cbz r0,0x0007db96
  0007db8e: mov r0,r4
  0007db90: blx 0x0006edac
  0007db94: b 0x0007dbae
  0007db96: movw r0,#0x803a
  0007db9a: blx 0x0006edb8
  0007db9e: mov.w r0,#0xb50
  0007dba2: blx 0x0006edc4
  0007dba6: mov.w r0,#0x3f800000
  0007dbaa: blx 0x0006edd0
  0007dbae: add.w r0,r4,#0x468
  0007dbb2: add r1,sp,#0x10
  0007dbb4: mov.w r6,#0x3f800000
  0007dbb8: movs r5,#0x0
  0007dbba: str r6,[sp,#0x14]
  0007dbbc: str r5,[sp,#0x10]
  0007dbbe: str r5,[sp,#0x18]
  0007dbc0: blx 0x0006eb3c
  0007dbc4: addw r0,r4,#0x474
  0007dbc8: add r1,sp,#0x10
  0007dbca: str.w r5,[r4,#0x378]
  0007dbce: strd r5,r6,[sp,#0x10]
  0007dbd2: str r5,[sp,#0x18]
  0007dbd4: blx 0x0006eb3c
  0007dbd8: movw r0,#0xb71
  0007dbdc: str.w r5,[r4,#0x37c]
  0007dbe0: blx 0x0006edb8
  0007dbe4: mov r0,r4
  0007dbe6: movw r1,#0xde1
  0007dbea: movs r2,#0x1
  0007dbec: blx 0x0006eddc
  0007dbf0: movw r0,#0xbe2
  0007dbf4: blx 0x0006edc4
  0007dbf8: movw r0,#0x405
  0007dbfc: blx 0x0006ede8
  0007dc00: movw r0,#0xb44
  0007dc04: blx 0x0006edb8
  0007dc08: mov r0,r4
  0007dc0a: blx 0x0006edf4
  0007dc0e: ldr r0,[r4,#0x30]
  0007dc10: movs r1,#0x0
  0007dc12: ldr r0,[r0,#0x0]
  0007dc14: blx 0x0006ee00
  0007dc18: mov r1,r4
  0007dc1a: movw r0,#0xd33
  0007dc1e: str.w r5,[r1,#0xc]!
  0007dc22: blx 0x0006ee0c
  0007dc26: ldrb.w r0,[r10,#0x0]
  0007dc2a: cbz r0,0x0007dc70
  0007dc2c: ldr r0,[0x0007dcb0]
  0007dc2e: add r0,pc
  0007dc30: ldr r0,[r0,#0x0]
  0007dc32: ldrb r0,[r0,#0x0]
  0007dc34: cbz r0,0x0007dc70
  0007dc36: movs r0,#0x38
  0007dc38: blx 0x0006eb24
  0007dc3c: ldr r1,[0x0007dcb4]
  0007dc3e: mov r5,r0
  0007dc40: add r1,pc
  0007dc42: add r0,sp,#0x4
  0007dc44: movs r2,#0x0
  0007dc46: blx 0x0006ee18
  0007dc4a: add r2,sp,#0x4
  0007dc4c: mov r0,r5
  0007dc4e: mov r1,r4
  0007dc50: blx 0x0006ee24
  0007dc54: add r0,sp,#0x4
  0007dc56: str.w r5,[r4,#0x418]
  0007dc5a: blx 0x0006ee30
  0007dc5e: ldrd r1,r2,[r4,#0x368]
  0007dc62: movs r3,#0x0
  0007dc64: ldr.w r0,[r4,#0x418]
  0007dc68: str r3,[sp,#0x0]
  0007dc6a: movs r3,#0x1
  0007dc6c: blx 0x0006ee3c
  0007dc70: ldr r0,[sp,#0x1c]
  0007dc72: ldr.w r1,[r8,#0x0]
  0007dc76: subs r0,r1,r0
  0007dc78: itttt eq
  0007dc7a: mov.eq r0,#0x1
  0007dc7c: add.eq sp,#0x20
  0007dc7e: pop.eq.w {r8,r9,r10}
  0007dc82: pop.eq {r4,r5,r6,r7,pc}
  0007dc84: blx 0x0006e824
```
