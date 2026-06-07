# MeshMerger::~MeshMerger
elf 0x174078 body 172
Sig: undefined __thiscall ~MeshMerger(MeshMerger * this)

## decompile
```c

/* MeshMerger::~MeshMerger() */

MeshMerger * __thiscall MeshMerger::~MeshMerger(MeshMerger *this)

{
  void *pvVar1;
  int *piVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  code *pcVar6;
  
  if (*(void **)(this + 8) != (void *)0x0) {
    operator_delete__(*(void **)(this + 8));
  }
  iVar5 = 0;
  iVar4 = 0;
  *(undefined4 *)(this + 8) = 0;
  while( true ) {
    pvVar1 = *(void **)(this + 0x18);
    if (*(int *)(this + 0x30) * *(int *)this <= iVar4) break;
    puVar3 = (undefined4 *)(*(int *)((int)pvVar1 + iVar5) + 4);
    if ((void *)*puVar3 == (void *)0x0) {
      piVar2 = (int *)((int)pvVar1 + iVar4 * 4);
    }
    else {
      operator_delete__((void *)*puVar3);
      piVar2 = (int *)(*(int *)(this + 0x18) + iVar5);
      puVar3 = (undefined4 *)(*(int *)(*(int *)(this + 0x18) + iVar5) + 4);
    }
    *puVar3 = 0;
    puVar3 = (undefined4 *)(*piVar2 + 0x10);
    if ((void *)*puVar3 != (void *)0x0) {
      operator_delete__((void *)*puVar3);
      piVar2 = (int *)(*(int *)(this + 0x18) + iVar5);
      puVar3 = (undefined4 *)(*(int *)(*(int *)(this + 0x18) + iVar5) + 0x10);
    }
    *puVar3 = 0;
    if ((void *)*piVar2 != (void *)0x0) {
      operator_delete((void *)*piVar2);
      piVar2 = (int *)(*(int *)(this + 0x18) + iVar5);
    }
    *piVar2 = 0;
    iVar5 = iVar5 + 4;
    iVar4 = iVar4 + 1;
  }
  if (pvVar1 != (void *)0x0) {
    operator_delete__(pvVar1);
  }
  iVar4 = DAT_00184124;
  *(undefined4 *)(this + 0x18) = 0;
  pcVar6 = *(code **)(iVar4 + 0x184100);
  (*pcVar6)(*(undefined4 *)(this + 0x24));
  *(undefined4 *)(this + 0x24) = 0;
  (*pcVar6)(*(undefined4 *)(this + 0x28));
  *(undefined4 *)(this + 0x28) = 0;
  (*pcVar6)(*(undefined4 *)(this + 0x2c));
  *(undefined4 *)(this + 0x2c) = 0;
  if (*(void **)(this + 0x1c) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x1c));
  }
  *(undefined4 *)(this + 0x1c) = 0;
  return this;
}

```

## target disasm
```
  00184078: push {r4,r5,r6,r7,lr}
  0018407a: add r7,sp,#0xc
  0018407c: push.w r8
  00184080: mov r4,r0
  00184082: ldr r0,[r0,#0x8]
  00184084: cbz r0,0x0018408a
  00184086: blx 0x0006ebfc
  0018408a: mov.w r8,#0x0
  0018408e: movs r6,#0x0
  00184090: movs r5,#0x0
  00184092: str.w r8,[r4,#0x8]
  00184096: b 0x001840e4
  00184098: ldr r2,[r0,r6]
  0018409a: ldr.w r1,[r2,#0x4]!
  0018409e: cbz r1,0x001840b0
  001840a0: mov r0,r1
  001840a2: blx 0x0006ebfc
  001840a6: ldr r0,[r4,#0x18]
  001840a8: adds r1,r0,r6
  001840aa: ldr r2,[r0,r6]
  001840ac: adds r2,#0x4
  001840ae: b 0x001840b4
  001840b0: add.w r1,r0,r5, lsl #0x2
  001840b4: str.w r8,[r2,#0x0]
  001840b8: ldr r2,[r1,#0x0]
  001840ba: ldr.w r0,[r2,#0x10]!
  001840be: cbz r0,0x001840cc
  001840c0: blx 0x0006ebfc
  001840c4: ldr r0,[r4,#0x18]
  001840c6: adds r1,r0,r6
  001840c8: ldr r2,[r0,r6]
  001840ca: adds r2,#0x10
  001840cc: str.w r8,[r2,#0x0]
  001840d0: ldr r0,[r1,#0x0]
  001840d2: cbz r0,0x001840dc
  001840d4: blx 0x0006eb48
  001840d8: ldr r0,[r4,#0x18]
  001840da: adds r1,r0,r6
  001840dc: str.w r8,[r1,#0x0]
  001840e0: adds r6,#0x4
  001840e2: adds r5,#0x1
  001840e4: ldr r1,[r4,#0x0]
  001840e6: ldr r2,[r4,#0x30]
  001840e8: ldr r0,[r4,#0x18]
  001840ea: muls r1,r2
  001840ec: cmp r5,r1
  001840ee: blt 0x00184098
  001840f0: cbz r0,0x001840f6
  001840f2: blx 0x0006ebfc
  001840f6: ldr r1,[0x00184124]
  001840f8: movs r5,#0x0
  001840fa: str r5,[r4,#0x18]
  001840fc: add r1,pc
  001840fe: ldr r0,[r4,#0x24]
  00184100: ldr r6,[r1,#0x0]
  00184102: blx r6
  00184104: ldr r0,[r4,#0x28]
  00184106: str r5,[r4,#0x24]
  00184108: blx r6
  0018410a: ldr r0,[r4,#0x2c]
  0018410c: str r5,[r4,#0x28]
  0018410e: blx r6
  00184110: ldr r0,[r4,#0x1c]
  00184112: str r5,[r4,#0x2c]
  00184114: cbz r0,0x0018411a
  00184116: blx 0x0006ebfc
  0018411a: str r5,[r4,#0x1c]
  0018411c: mov r0,r4
  0018411e: pop.w r8
  00184122: pop {r4,r5,r6,r7,pc}
```
