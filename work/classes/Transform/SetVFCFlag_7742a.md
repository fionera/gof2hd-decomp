# Transform::SetVFCFlag
elf 0x7742a body 78
Sig: undefined __thiscall SetVFCFlag(Transform * this, bool param_1)

## decompile
```c

/* AbyssEngine::Transform::SetVFCFlag(bool) */

void __thiscall AbyssEngine::Transform::SetVFCFlag(Transform *this,bool param_1)

{
  int iVar1;
  Transform *pTVar2;
  uint uVar3;
  
  for (uVar3 = 0; uVar3 < *(uint *)(this + 0x3c); uVar3 = uVar3 + 1) {
    iVar1 = *(int *)(*(int *)(this + 0x40) + uVar3 * 4);
    if ((iVar1 != 0) && (pTVar2 = *(Transform **)(iVar1 + 0x34), pTVar2 != (Transform *)0x0)) {
      SetVFCFlag(pTVar2,param_1);
    }
  }
  for (uVar3 = 0; uVar3 < *(uint *)(this + 0x4c); uVar3 = uVar3 + 1) {
    pTVar2 = *(Transform **)(*(int *)(this + 0x50) + uVar3 * 4);
    if (pTVar2 != (Transform *)0x0) {
      SetVFCFlag(pTVar2,param_1);
    }
  }
  this[0x17c] = (Transform)param_1;
  return;
}

```

## target disasm
```
  0008742a: push {r4,r5,r6,r7,lr}
  0008742c: add r7,sp,#0xc
  0008742e: push.w r11
  00087432: mov r4,r1
  00087434: mov r5,r0
  00087436: movs r6,#0x0
  00087438: b 0x0008744e
  0008743a: ldr r0,[r5,#0x40]
  0008743c: ldr.w r0,[r0,r6,lsl #0x2]
  00087440: cbz r0,0x0008744c
  00087442: ldr r0,[r0,#0x34]
  00087444: cbz r0,0x0008744c
  00087446: mov r1,r4
  00087448: blx 0x0006f868
  0008744c: adds r6,#0x1
  0008744e: ldr r0,[r5,#0x3c]
  00087450: cmp r6,r0
  00087452: bcc 0x0008743a
  00087454: movs r6,#0x0
  00087456: b 0x00087468
  00087458: ldr r0,[r5,#0x50]
  0008745a: ldr.w r0,[r0,r6,lsl #0x2]
  0008745e: cbz r0,0x00087466
  00087460: mov r1,r4
  00087462: blx 0x0006f868
  00087466: adds r6,#0x1
  00087468: ldr r0,[r5,#0x4c]
  0008746a: cmp r6,r0
  0008746c: bcc 0x00087458
  0008746e: strb.w r4,[r5,#0x17c]
  00087472: pop.w r11
  00087476: pop {r4,r5,r6,r7,pc}
```
