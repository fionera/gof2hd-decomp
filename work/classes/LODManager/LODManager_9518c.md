# LODManager::LODManager
elf 0x9518c body 38
Sig: undefined __thiscall LODManager(LODManager * this)

## decompile
```c

/* LODManager::LODManager() */

LODManager * __thiscall LODManager::LODManager(LODManager *this)

{
  Array<AEGeometry*> *this_00;
  
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  this_00 = operator_new(0xc);
  Array<AEGeometry*>::Array(this_00);
  *(undefined4 *)(this + 0x10) = 0x3e9;
  *(Array<AEGeometry*> **)this = this_00;
  return this;
}

```

## target disasm
```
  000a518c: push {r4,r5,r7,lr}
  000a518e: add r7,sp,#0x8
  000a5190: mov r4,r0
  000a5192: movs r0,#0x0
  000a5194: strd r0,r0,[r4,#0x4]
  000a5198: str r0,[r4,#0xc]
  000a519a: movs r0,#0xc
  000a519c: blx 0x0006eb24
  000a51a0: mov r5,r0
  000a51a2: blx 0x000717b8
  000a51a6: movw r0,#0x3e9
  000a51aa: str r0,[r4,#0x10]
  000a51ac: mov r0,r4
  000a51ae: str r5,[r4,#0x0]
  000a51b0: pop {r4,r5,r7,pc}
```
