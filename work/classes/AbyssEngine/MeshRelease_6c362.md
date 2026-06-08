# AbyssEngine::MeshRelease
elf 0x6c362 body 32
Sig: undefined __stdcall MeshRelease(Engine * param_1, Mesh * * param_2)

## decompile
```c

/* AbyssEngine::MeshRelease(AbyssEngine::Engine*, AbyssEngine::Mesh**) */

void AbyssEngine::MeshRelease(Engine *param_1,Mesh **param_2)

{
  undefined4 uVar1;
  
  if ((param_1 != (Engine *)0x0) && (*param_2 != (Mesh *)0x0)) {
    uVar1 = TransformRelease(param_1,(Transform **)(*param_2 + 0x34));
    (*(code *)(DAT_001ab034 + 0x1ab038))(uVar1,param_2);
    return;
  }
  return;
}

```

## target disasm
```
  0007c362: push {r4,r6,r7,lr}
  0007c364: add r7,sp,#0x8
  0007c366: mov r4,r1
  0007c368: cbz r0,0x0007c37e
  0007c36a: ldr r1,[r4,#0x0]
  0007c36c: cbz r1,0x0007c37e
  0007c36e: adds r1,#0x34
  0007c370: blx 0x0006ebcc
  0007c374: mov r1,r4
  0007c376: pop.w {r4,r6,r7,lr}
  0007c37a: b.w 0x001ab028
  0007c37e: pop {r4,r6,r7,pc}
  001ab028: bx pc
```
