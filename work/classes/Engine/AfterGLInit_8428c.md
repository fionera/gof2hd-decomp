# Engine::AfterGLInit
elf 0x8428c body 154
Sig: undefined __thiscall AfterGLInit(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::AfterGLInit() */

void __thiscall AbyssEngine::Engine::AfterGLInit(Engine *this)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  char *pcVar3;
  int *piVar4;
  String aSStack_20 [12];
  int local_14;
  
  piVar4 = *(int **)(DAT_0009433c + 0x9429a);
  local_14 = *piVar4;
  ResetLightParam(this);
  MeshCreate(this,4,2,0x13,this + 0x380);
  uVar1 = DAT_00094338;
  puVar2 = *(undefined4 **)(*(int *)(this + 0x380) + 0x2c);
  *puVar2 = 0x20000;
  puVar2[1] = 1;
  puVar2[2] = uVar1;
  pcVar3 = (char *)glGetString(0x1f00);
  String::String(aSStack_20,pcVar3,false);
  String::operator=(*(String **)(DAT_00094340 + 0x942e4),aSStack_20);
  String::~String(aSStack_20);
  pcVar3 = (char *)glGetString(0x1f01);
  String::String(aSStack_20,pcVar3,false);
  String::operator=(*(String **)(DAT_00094344 + 0x9430a),aSStack_20);
  String::~String(aSStack_20);
  if (*piVar4 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0009428c: push {r4,r5,r7,lr}
  0009428e: add r7,sp,#0x8
  00094290: sub sp,#0x18
  00094292: mov r4,r0
  00094294: ldr r0,[0x0009433c]
  00094296: add r0,pc
  00094298: ldr r5,[r0,#0x0]
  0009429a: ldr r0,[r5,#0x0]
  0009429c: str r0,[sp,#0x14]
  0009429e: mov r0,r4
  000942a0: blx 0x0006ed94
  000942a4: add.w r0,r4,#0x380
  000942a8: str r0,[sp,#0x0]
  000942aa: mov r0,r4
  000942ac: movs r1,#0x4
  000942ae: movs r2,#0x2
  000942b0: movs r3,#0x13
  000942b2: blx 0x0006f1fc
  000942b6: ldr.w r0,[r4,#0x380]
  000942ba: mov.w r2,#0x20000
  000942be: ldr r1,[0x00094338]
  000942c0: ldr r0,[r0,#0x2c]
  000942c2: str r2,[r0,#0x0]
  000942c4: movs r2,#0x1
  000942c6: str r2,[r0,#0x4]
  000942c8: str r1,[r0,#0x8]
  000942ca: mov.w r0,#0x1f00
  000942ce: blx 0x00070528
  000942d2: add r4,sp,#0x8
  000942d4: mov r1,r0
  000942d6: movs r2,#0x0
  000942d8: mov r0,r4
  000942da: blx 0x0006ee18
  000942de: ldr r0,[0x00094340]
  000942e0: add r0,pc
  000942e2: ldr r0,[r0,#0x0]
  000942e4: mov r1,r4
  000942e6: blx 0x0006f2b0
  000942ea: add r0,sp,#0x8
  000942ec: blx 0x0006ee30
  000942f0: movw r0,#0x1f01
  000942f4: blx 0x00070528
  000942f8: add r4,sp,#0x8
  000942fa: mov r1,r0
  000942fc: movs r2,#0x0
  000942fe: mov r0,r4
  00094300: blx 0x0006ee18
  00094304: ldr r0,[0x00094344]
  00094306: add r0,pc
  00094308: ldr r0,[r0,#0x0]
  0009430a: mov r1,r4
  0009430c: blx 0x0006f2b0
  00094310: add r0,sp,#0x8
  00094312: blx 0x0006ee30
  00094316: ldr r0,[sp,#0x14]
  00094318: ldr r1,[r5,#0x0]
  0009431a: subs r0,r1,r0
  0009431c: itt eq
  0009431e: add.eq sp,#0x18
  00094320: pop.eq {r4,r5,r7,pc}
  00094322: blx 0x0006e824
```
