# LODManager::removeObject
elf 0x9524e body 48
Sig: undefined __thiscall removeObject(LODManager * this, AEGeometry * param_1)

## decompile
```c

/* LODManager::removeObject(AEGeometry*) */

void __thiscall LODManager::removeObject(LODManager *this,AEGeometry *param_1)

{
  Array *pAVar1;
  uint uVar2;
  
  for (uVar2 = 0; pAVar1 = *(Array **)this, uVar2 < *(uint *)pAVar1; uVar2 = uVar2 + 1) {
    if (*(AEGeometry **)(*(int *)(pAVar1 + 4) + uVar2 * 4) == param_1) {
      ArrayRemove<AEGeometry*>(param_1,pAVar1);
    }
  }
  return;
}

```

## target disasm
```
  000a524e: push {r4,r5,r6,r7,lr}
  000a5250: add r7,sp,#0xc
  000a5252: push.w r11
  000a5256: mov r4,r1
  000a5258: mov r5,r0
  000a525a: movs r6,#0x0
  000a525c: b 0x000a5270
  000a525e: ldr r0,[r1,#0x4]
  000a5260: ldr.w r0,[r0,r6,lsl #0x2]
  000a5264: cmp r0,r4
  000a5266: itt eq
  000a5268: mov.eq r0,r4
  000a526a: blx.eq 0x000717e8
  000a526e: adds r6,#0x1
  000a5270: ldr r1,[r5,#0x0]
  000a5272: ldr r0,[r1,#0x0]
  000a5274: cmp r6,r0
  000a5276: bcc 0x000a525e
  000a5278: pop.w r11
  000a527c: pop {r4,r5,r6,r7,pc}
```
