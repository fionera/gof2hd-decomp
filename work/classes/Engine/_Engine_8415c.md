# Engine::~Engine
elf 0x8415c body 168
Sig: undefined __thiscall ~Engine(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::~Engine() */

void __thiscall AbyssEngine::Engine::~Engine(Engine *this)

{
  void *pvVar1;
  code *UNRECOVERED_JUMPTABLE;
  
  if (*(code **)(this + 0x484) != (code *)0x0) {
    (**(code **)(this + 0x484))(this);
  }
  if (*(ApplicationManager **)(this + 0x30) != (ApplicationManager *)0x0) {
    pvVar1 = (void *)ApplicationManager::~ApplicationManager(*(ApplicationManager **)(this + 0x30));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x30) = 0;
  if (*(int **)(this + 0x24) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x24) + 4))();
  }
  *(undefined4 *)(this + 0x24) = 0;
  AEFile::Release();
  ArrayReleaseClasses<AbyssEngine::ShaderBaseStruct*>((Array *)(this + 0x510));
  if (*(FBOContainer **)(this + 0x414) != (FBOContainer *)0x0) {
    pvVar1 = (void *)FBOContainer::~FBOContainer(*(FBOContainer **)(this + 0x414));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x414) = 0;
  if (*(FBOContainer **)(this + 0x418) != (FBOContainer *)0x0) {
    pvVar1 = (void *)FBOContainer::~FBOContainer(*(FBOContainer **)(this + 0x418));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x418) = 0;
  MeshRelease(this,(Mesh **)(this + 0x380));
  ReleaseGL();
  Array<AbyssEngine::ShaderBaseStruct*>::~Array
            ((Array<AbyssEngine::ShaderBaseStruct*> *)(this + 0x510));
  Array<int>::~Array((Array<int> *)(this + 0x3d8));
  UNRECOVERED_JUMPTABLE = *(code **)(DAT_00094238 + 0x941e4);
  (*UNRECOVERED_JUMPTABLE)(this + 0x4c);
  (*UNRECOVERED_JUMPTABLE)(this + 0x3c);
  (*UNRECOVERED_JUMPTABLE)(this + 0x14);
                    /* WARNING: Could not recover jumptable at 0x00094202. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(this);
  return;
}

```

## target disasm
```
  0009415c: push {r4,r5,r6,r7,lr}
  0009415e: add r7,sp,#0xc
  00094160: push.w r11
  00094164: ldr.w r1,[r0,#0x484]
  00094168: mov r4,r0
  0009416a: cbz r1,0x00094170
  0009416c: mov r0,r4
  0009416e: blx r1
  00094170: ldr r0,[r4,#0x30]
  00094172: cbz r0,0x0009417c
  00094174: blx 0x000704ec
  00094178: blx 0x0006eb48
  0009417c: ldr r0,[r4,#0x24]
  0009417e: movs r5,#0x0
  00094180: str r5,[r4,#0x30]
  00094182: cbz r0,0x0009418a
  00094184: ldr r1,[r0,#0x0]
  00094186: ldr r1,[r1,#0x4]
  00094188: blx r1
  0009418a: str r5,[r4,#0x24]
  0009418c: blx 0x000704f8
  00094190: add.w r5,r4,#0x510
  00094194: mov r0,r5
  00094196: blx 0x00070504
  0009419a: ldr.w r0,[r4,#0x414]
  0009419e: cbz r0,0x000941a8
  000941a0: blx 0x00070510
  000941a4: blx 0x0006eb48
  000941a8: ldr.w r0,[r4,#0x418]
  000941ac: movs r6,#0x0
  000941ae: str.w r6,[r4,#0x414]
  000941b2: cbz r0,0x000941bc
  000941b4: blx 0x00070510
  000941b8: blx 0x0006eb48
  000941bc: str.w r6,[r4,#0x418]
  000941c0: add.w r1,r4,#0x380
  000941c4: mov r0,r4
  000941c6: blx 0x0006ebd8
  000941ca: mov r0,r4
  000941cc: blx 0x0007051c
  000941d0: mov r0,r5
  000941d2: blx 0x000704e0
  000941d6: add.w r0,r4,#0x3d8
  000941da: blx 0x00070204
  000941de: ldr r0,[0x00094238]
  000941e0: add r0,pc
  000941e2: ldr r5,[r0,#0x0]
  000941e4: add.w r0,r4,#0x4c
  000941e8: blx r5
  000941ea: add.w r0,r4,#0x3c
  000941ee: blx r5
  000941f0: add.w r0,r4,#0x14
  000941f4: blx r5
  000941f6: mov r0,r4
  000941f8: mov r1,r5
  000941fa: pop.w r11
  000941fe: pop.w {r4,r5,r6,r7,lr}
  00094202: bx r1
```
