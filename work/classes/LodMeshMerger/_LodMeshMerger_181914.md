# LodMeshMerger::~LodMeshMerger
elf 0x181914 body 168
Sig: undefined __thiscall ~LodMeshMerger(LodMeshMerger * this)

## decompile
```c

/* LodMeshMerger::~LodMeshMerger() */

void __thiscall LodMeshMerger::~LodMeshMerger(LodMeshMerger *this)

{
  void *pvVar1;
  int *piVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  code *pcVar6;
  
  iVar5 = 0;
  for (iVar4 = 0; pvVar1 = *(void **)(this + 0x24), iVar4 < *(int *)(this + 0x38) * *(int *)this;
      iVar4 = iVar4 + 1) {
    puVar3 = (undefined4 *)(*(int *)((int)pvVar1 + iVar5) + 4);
    if ((void *)*puVar3 == (void *)0x0) {
      piVar2 = (int *)((int)pvVar1 + iVar4 * 4);
    }
    else {
      operator_delete__((void *)*puVar3);
      piVar2 = (int *)(*(int *)(this + 0x24) + iVar5);
      puVar3 = (undefined4 *)(*(int *)(*(int *)(this + 0x24) + iVar5) + 4);
    }
    *puVar3 = 0;
    puVar3 = (undefined4 *)(*piVar2 + 0x10);
    if ((void *)*puVar3 != (void *)0x0) {
      operator_delete__((void *)*puVar3);
      piVar2 = (int *)(*(int *)(this + 0x24) + iVar5);
      puVar3 = (undefined4 *)(*(int *)(*(int *)(this + 0x24) + iVar5) + 0x10);
    }
    *puVar3 = 0;
    if ((void *)*piVar2 != (void *)0x0) {
      operator_delete((void *)*piVar2);
      piVar2 = (int *)(*(int *)(this + 0x24) + iVar5);
    }
    *piVar2 = 0;
    iVar5 = iVar5 + 4;
  }
  if (pvVar1 != (void *)0x0) {
    operator_delete__(pvVar1);
  }
  iVar4 = DAT_001919bc;
  *(undefined4 *)(this + 0x24) = 0;
  pcVar6 = *(code **)(iVar4 + 0x191990);
  (*pcVar6)(*(undefined4 *)(this + 0x2c));
  *(undefined4 *)(this + 0x2c) = 0;
  (*pcVar6)(*(undefined4 *)(this + 0x30));
  *(undefined4 *)(this + 0x30) = 0;
  (*pcVar6)(*(undefined4 *)(this + 0x34));
  *(undefined4 *)(this + 0x34) = 0;
  if (*(void **)(this + 0x28) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x28));
  }
  *(undefined4 *)(this + 0x28) = 0;
  (*(code *)(DAT_001ac894 + 0x1ac898))(this);
  return;
}

```

## target disasm
```
  00191914: push {r4,r5,r6,r7,lr}
  00191916: add r7,sp,#0xc
  00191918: push.w r8
  0019191c: mov r4,r0
  0019191e: mov.w r8,#0x0
  00191922: movs r6,#0x0
  00191924: movs r5,#0x0
  00191926: b 0x00191974
  00191928: ldr r2,[r0,r6]
  0019192a: ldr.w r1,[r2,#0x4]!
  0019192e: cbz r1,0x00191940
  00191930: mov r0,r1
  00191932: blx 0x0006ebfc
  00191936: ldr r0,[r4,#0x24]
  00191938: adds r1,r0,r6
  0019193a: ldr r2,[r0,r6]
  0019193c: adds r2,#0x4
  0019193e: b 0x00191944
  00191940: add.w r1,r0,r5, lsl #0x2
  00191944: str.w r8,[r2,#0x0]
  00191948: ldr r2,[r1,#0x0]
  0019194a: ldr.w r0,[r2,#0x10]!
  0019194e: cbz r0,0x0019195c
  00191950: blx 0x0006ebfc
  00191954: ldr r0,[r4,#0x24]
  00191956: adds r1,r0,r6
  00191958: ldr r2,[r0,r6]
  0019195a: adds r2,#0x10
  0019195c: str.w r8,[r2,#0x0]
  00191960: ldr r0,[r1,#0x0]
  00191962: cbz r0,0x0019196c
  00191964: blx 0x0006eb48
  00191968: ldr r0,[r4,#0x24]
  0019196a: adds r1,r0,r6
  0019196c: str.w r8,[r1,#0x0]
  00191970: adds r6,#0x4
  00191972: adds r5,#0x1
  00191974: ldr r1,[r4,#0x0]
  00191976: ldr r2,[r4,#0x38]
  00191978: ldr r0,[r4,#0x24]
  0019197a: muls r1,r2
  0019197c: cmp r5,r1
  0019197e: blt 0x00191928
  00191980: cbz r0,0x00191986
  00191982: blx 0x0006ebfc
  00191986: ldr r1,[0x001919bc]
  00191988: movs r5,#0x0
  0019198a: str r5,[r4,#0x24]
  0019198c: add r1,pc
  0019198e: ldr r0,[r4,#0x2c]
  00191990: ldr r6,[r1,#0x0]
  00191992: blx r6
  00191994: ldr r0,[r4,#0x30]
  00191996: str r5,[r4,#0x2c]
  00191998: blx r6
  0019199a: ldr r0,[r4,#0x34]
  0019199c: str r5,[r4,#0x30]
  0019199e: blx r6
  001919a0: ldr r0,[r4,#0x28]
  001919a2: str r5,[r4,#0x34]
  001919a4: cbz r0,0x001919aa
  001919a6: blx 0x0006ebfc
  001919aa: str r5,[r4,#0x28]
  001919ac: mov r0,r4
  001919ae: pop.w r8
  001919b2: pop.w {r4,r5,r6,r7,lr}
  001919b6: b.w 0x001ac888
  001ac888: bx pc
```
