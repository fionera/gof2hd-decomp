# HangarList::release
elf 0x11da10 body 108
Sig: undefined __thiscall release(HangarList * this)

## decompile
```c

/* HangarList::release() */

void __thiscall HangarList::release(HangarList *this)

{
  Array *pAVar1;
  Array<ListItem*> *this_00;
  undefined4 *puVar2;
  void *pvVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  pAVar1 = *(Array **)this;
  if (pAVar1 != (Array *)0x0) {
    iVar6 = 0;
    for (uVar5 = 0; uVar5 < *(uint *)pAVar1; uVar5 = uVar5 + 1) {
      iVar4 = *(int *)(pAVar1 + 4);
      this_00 = *(Array<ListItem*> **)(iVar4 + iVar6);
      if (this_00 == (Array<ListItem*> *)0x0) {
LAB_0012da4e:
        puVar2 = (undefined4 *)(iVar4 + uVar5 * 4);
      }
      else {
        if (*(int *)this_00 != 0) {
          ArrayReleaseClasses<ListItem*>((Array *)this_00);
          iVar4 = *(int *)(*(int *)this + 4);
          this_00 = *(Array<ListItem*> **)(iVar4 + iVar6);
          if (this_00 == (Array<ListItem*> *)0x0) goto LAB_0012da4e;
        }
        pvVar3 = (void *)Array<ListItem*>::~Array(this_00);
        operator_delete(pvVar3);
        puVar2 = (undefined4 *)(*(int *)(*(int *)this + 4) + iVar6);
      }
      *puVar2 = 0;
      iVar6 = iVar6 + 4;
      pAVar1 = *(Array **)this;
    }
    ArrayReleaseClasses<Array<ListItem*>*>(pAVar1);
    if (*(Array<Array<ListItem*>*> **)this != (Array<Array<ListItem*>*> *)0x0) {
      pvVar3 = (void *)Array<Array<ListItem*>*>::~Array(*(Array<Array<ListItem*>*> **)this);
      operator_delete(pvVar3);
    }
  }
  *(undefined4 *)this = 0;
  return;
}

```

## target disasm
```
  0012da10: push {r4,r5,r6,r7,lr}
  0012da12: add r7,sp,#0xc
  0012da14: push.w r8
  0012da18: mov r4,r0
  0012da1a: ldr r0,[r0,#0x0]
  0012da1c: cbz r0,0x0012da72
  0012da1e: mov.w r8,#0x0
  0012da22: movs r6,#0x0
  0012da24: movs r5,#0x0
  0012da26: b 0x0012da5c
  0012da28: ldr r1,[r0,#0x4]
  0012da2a: ldr r0,[r1,r6]
  0012da2c: cbz r0,0x0012da4e
  0012da2e: ldr r1,[r0,#0x0]
  0012da30: cbz r1,0x0012da3e
  0012da32: blx 0x00076108
  0012da36: ldr r0,[r4,#0x0]
  0012da38: ldr r1,[r0,#0x4]
  0012da3a: ldr r0,[r1,r6]
  0012da3c: cbz r0,0x0012da4e
  0012da3e: blx 0x00076114
  0012da42: blx 0x0006eb48
  0012da46: ldr r0,[r4,#0x0]
  0012da48: ldr r0,[r0,#0x4]
  0012da4a: add r0,r6
  0012da4c: b 0x0012da52
  0012da4e: add.w r0,r1,r5, lsl #0x2
  0012da52: str.w r8,[r0,#0x0]
  0012da56: adds r6,#0x4
  0012da58: ldr r0,[r4,#0x0]
  0012da5a: adds r5,#0x1
  0012da5c: ldr r1,[r0,#0x0]
  0012da5e: cmp r5,r1
  0012da60: bcc 0x0012da28
  0012da62: blx 0x00076120
  0012da66: ldr r0,[r4,#0x0]
  0012da68: cbz r0,0x0012da72
  0012da6a: blx 0x0007612c
  0012da6e: blx 0x0006eb48
  0012da72: movs r0,#0x0
  0012da74: str r0,[r4,#0x0]
  0012da76: pop.w r8
  0012da7a: pop {r4,r5,r6,r7,pc}
```
