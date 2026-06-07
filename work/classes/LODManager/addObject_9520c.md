# LODManager::addObject
elf 0x9520c body 32
Sig: undefined __thiscall addObject(LODManager * this, AEGeometry * param_1)

## decompile
```c

/* LODManager::addObject(AEGeometry*) */

void __thiscall LODManager::addObject(LODManager *this,AEGeometry *param_1)

{
  int iVar1;
  
  iVar1 = AEGeometry::hasLod(param_1);
  if (iVar1 == 0) {
    return;
  }
  (*(code *)(DAT_001ab8d4 + 0x1ab8d8))(param_1,*(undefined4 *)this);
  return;
}

```

## target disasm
```
  000a520c: push {r4,r5,r7,lr}
  000a520e: add r7,sp,#0x8
  000a5210: mov r5,r0
  000a5212: mov r0,r1
  000a5214: mov r4,r1
  000a5216: blx 0x000717d0
  000a521a: cbz r0,0x000a5228
  000a521c: ldr r1,[r5,#0x0]
  000a521e: mov r0,r4
  000a5220: pop.w {r4,r5,r7,lr}
  000a5224: b.w 0x001ab8c8
  000a5228: pop {r4,r5,r7,pc}
  001ab8c8: bx pc
```
