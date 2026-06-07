# BumpMapping::Init
elf 0x912f4 body 164
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpMapping::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpMapping::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a1398 + 0xa1302));
  iVar2 = DAT_000a13a4;
  puVar3 = (undefined4 *)(DAT_000a13a0 + 0xa130e);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0xa1312);
  iVar2 = DAT_000a13a8 + 0xa131c;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a13ac + 0xa1328;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a13b0 + 0xa1334;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a13b4 + 0xa1340;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_000a13b8 + 0xa134c);
  iVar2 = DAT_000a13bc + 0xa1350;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a13c0 + 0xa135e;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a13c4 + 0xa136a;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a13c8 + 0xa1376;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  glUniform1i(*(undefined4 *)(param_1 + 0x3c),0);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x40),1);
  return;
}

```

## target disasm
```
  000a12f4: push {r4,r5,r7,lr}
  000a12f6: add r7,sp,#0x8
  000a12f8: ldr r1,[0x000a1398]
  000a12fa: mov r4,r0
  000a12fc: ldr r2,[0x000a139c]
  000a12fe: add r1,pc
  000a1300: add r2,pc
  000a1302: blx 0x00070954
  000a1306: ldr r2,[0x000a13a0]
  000a1308: ldr r1,[0x000a13a4]
  000a130a: add r2,pc
  000a130c: str r0,[r4,#0x4]
  000a130e: add r1,pc
  000a1310: ldr r5,[r2,#0x0]
  000a1312: blx r5
  000a1314: ldr r1,[0x000a13a8]
  000a1316: ldr r2,[r4,#0x4]
  000a1318: add r1,pc
  000a131a: str r0,[r4,#0x20]
  000a131c: mov r0,r2
  000a131e: blx r5
  000a1320: ldr r1,[0x000a13ac]
  000a1322: ldr r2,[r4,#0x4]
  000a1324: add r1,pc
  000a1326: str r0,[r4,#0x24]
  000a1328: mov r0,r2
  000a132a: blx r5
  000a132c: ldr r1,[0x000a13b0]
  000a132e: ldr r2,[r4,#0x4]
  000a1330: add r1,pc
  000a1332: str r0,[r4,#0x28]
  000a1334: mov r0,r2
  000a1336: blx r5
  000a1338: ldr r1,[0x000a13b4]
  000a133a: ldr r2,[r4,#0x4]
  000a133c: add r1,pc
  000a133e: str r0,[r4,#0x2c]
  000a1340: mov r0,r2
  000a1342: blx r5
  000a1344: ldr r3,[0x000a13b8]
  000a1346: ldr r1,[0x000a13bc]
  000a1348: add r3,pc
  000a134a: ldr r2,[r4,#0x4]
  000a134c: add r1,pc
  000a134e: str r0,[r4,#0x30]
  000a1350: ldr r5,[r3,#0x0]
  000a1352: mov r0,r2
  000a1354: blx r5
  000a1356: ldr r1,[0x000a13c0]
  000a1358: ldr r2,[r4,#0x4]
  000a135a: add r1,pc
  000a135c: str r0,[r4,#0x34]
  000a135e: mov r0,r2
  000a1360: blx r5
  000a1362: ldr r1,[0x000a13c4]
  000a1364: ldr r2,[r4,#0x4]
  000a1366: add r1,pc
  000a1368: str r0,[r4,#0x38]
  000a136a: mov r0,r2
  000a136c: blx r5
  000a136e: ldr r1,[0x000a13c8]
  000a1370: ldr r2,[r4,#0x4]
  000a1372: add r1,pc
  000a1374: str r0,[r4,#0x3c]
  000a1376: mov r0,r2
  000a1378: blx r5
  000a137a: ldr r1,[r4,#0x4]
  000a137c: str r0,[r4,#0x40]
  000a137e: mov r0,r1
  000a1380: blx 0x00070960
  000a1384: ldr r0,[r4,#0x3c]
  000a1386: movs r1,#0x0
  000a1388: blx 0x0007096c
  000a138c: ldr r0,[r4,#0x40]
  000a138e: movs r1,#0x1
  000a1390: pop.w {r4,r5,r7,lr}
  000a1394: b.w 0x001ab768
```
