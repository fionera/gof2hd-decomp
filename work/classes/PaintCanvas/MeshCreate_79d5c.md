# PaintCanvas::MeshCreate
elf 0x79d5c body 216
Sig: undefined __thiscall MeshCreate(PaintCanvas * this, ushort param_1, uint * param_2, bool param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshCreate(unsigned short, unsigned int&, bool) */

void __thiscall
AbyssEngine::PaintCanvas::MeshCreate(PaintCanvas *this,ushort param_1,uint *param_2,bool param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  Mesh *this_00;
  Material *pMVar4;
  undefined4 *puVar5;
  int *piVar6;
  int iVar7;
  
  piVar6 = *(int **)(DAT_00089e44 + 0x89d70);
  iVar1 = *piVar6;
  iVar2 = FindResource(this,param_1);
  if (iVar2 != 0) {
    uVar3 = *(uint *)(iVar2 + 8);
    if (uVar3 == 0xffffffff) {
      puVar5 = *(undefined4 **)(iVar2 + 0xc);
      MaterialCreate(this,*(ushort *)(puVar5 + 1),(uint *)&stack0xffffffd8);
      pMVar4 = (Material *)0x0;
      if (0xfffffffe < *(uint *)(this + 0x174)) {
        pMVar4 = *(Material **)(*(int *)(this + 0x178) + -4);
      }
      iVar7 = MeshCreateFromFile(*(Engine **)(this + 0x34),(char *)*puVar5,(Mesh **)&stack0xffffffd4
                                 ,pMVar4);
      if (iVar7 != 1) goto LAB_00089e20;
      if (**(char **)(DAT_00089e48 + 0x89dfc) != '\0') {
        uRam00000084 = 1;
        AbyssEngine::MeshConvertToVBO((Mesh *)0x0);
      }
      ArrayAdd<AbyssEngine::Mesh*>((Mesh *)0x0,(Array *)(this + 0x24));
      uVar3 = *(int *)(this + 0x24) - 1;
      *(uint *)(iVar2 + 8) = uVar3;
    }
    else {
      iVar7 = *(int *)(this + 0x28);
      if ((*(int *)(*(int *)(iVar7 + uVar3 * 4) + 0x34) != 0) || (param_3)) {
        this_00 = operator_new(0x88);
        Mesh::Mesh(this_00,*(Mesh **)(iVar7 + *(int *)(iVar2 + 8) * 4));
        ArrayAdd<AbyssEngine::Mesh*>(this_00,(Array *)(this + 0x24));
        uVar3 = *(int *)(this + 0x24) - 1;
      }
    }
    *param_2 = uVar3;
  }
LAB_00089e20:
  iVar2 = *piVar6;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  00089d5c: push {r4,r5,r6,r7,lr}
  00089d5e: add r7,sp,#0xc
  00089d60: push {r4,r5,r6,r7,r8,r9,r10}
  00089d64: mov r5,r0
  00089d66: ldr r0,[0x00089e44]
  00089d68: mov r4,r3
  00089d6a: mov r8,r2
  00089d6c: add r0,pc
  00089d6e: ldr.w r9,[r0,#0x0]
  00089d72: ldr.w r0,[r9,#0x0]
  00089d76: str r0,[sp,#0xc]
  00089d78: mov r0,r5
  00089d7a: blx 0x0006fb98
  00089d7e: mov r6,r0
  00089d80: cmp r0,#0x0
  00089d82: beq 0x00089e20
  00089d84: ldr r0,[r6,#0x8]
  00089d86: adds r1,r0,#0x1
  00089d88: beq 0x00089dbe
  00089d8a: ldr.w r10,[r5,#0x28]
  00089d8e: ldr.w r1,[r10,r0,lsl #0x2]
  00089d92: ldr r1,[r1,#0x34]
  00089d94: cbnz r1,0x00089d9a
  00089d96: cmp r4,#0x0
  00089d98: beq 0x00089e1c
  00089d9a: movs r0,#0x88
  00089d9c: blx 0x0006eb24
  00089da0: mov r4,r0
  00089da2: ldr r0,[r6,#0x8]
  00089da4: ldr.w r1,[r10,r0,lsl #0x2]
  00089da8: mov r0,r4
  00089daa: blx 0x0006f76c
  00089dae: adds r5,#0x24
  00089db0: mov r0,r4
  00089db2: mov r1,r5
  00089db4: blx 0x0006ecbc
  00089db8: ldr r0,[r5,#0x0]
  00089dba: subs r0,#0x1
  00089dbc: b 0x00089e1c
  00089dbe: ldr r4,[r6,#0xc]
  00089dc0: mov.w r0,#0xffffffff
  00089dc4: str r0,[sp,#0x8]
  00089dc6: add r2,sp,#0x8
  00089dc8: mov r0,r5
  00089dca: ldrh r1,[r4,#0x4]
  00089dcc: blx 0x0006fc1c
  00089dd0: movs r3,#0x0
  00089dd2: ldr.w r12,[sp,#0x8]
  00089dd6: str r3,[sp,#0x4]
  00089dd8: ldr.w r2,[r5,#0x174]
  00089ddc: ldr r0,[r5,#0x34]
  00089dde: ldr r1,[r4,#0x0]
  00089de0: cmp r12,r2
  00089de2: itt ls
  00089de4: ldr.ls.w r2,[r5,#0x178]
  00089de8: ldr.ls.w r3,[r2,r12,lsl #0x2]
  00089dec: add r2,sp,#0x4
  00089dee: blx 0x0006fc28
  00089df2: cmp r0,#0x1
  00089df4: bne 0x00089e20
  00089df6: ldr r0,[0x00089e48]
  00089df8: add r0,pc
  00089dfa: ldr r0,[r0,#0x0]
  00089dfc: ldrb r0,[r0,#0x0]
  00089dfe: cbz r0,0x00089e0c
  00089e00: ldr r0,[sp,#0x4]
  00089e02: movs r1,#0x1
  00089e04: strb.w r1,[r0,#0x84]
  00089e08: blx 0x0006eb0c
  00089e0c: ldr r0,[sp,#0x4]
  00089e0e: add.w r1,r5,#0x24
  00089e12: blx 0x0006ecbc
  00089e16: ldr r0,[r5,#0x24]
  00089e18: subs r0,#0x1
  00089e1a: str r0,[r6,#0x8]
  00089e1c: str.w r0,[r8,#0x0]
  00089e20: ldr r0,[sp,#0xc]
  00089e22: ldr.w r1,[r9,#0x0]
  00089e26: subs r0,r1,r0
  00089e28: itt eq
  00089e2a: pop.eq.w {r0,r1,r2,r3,r8,r9,r10}
  00089e2e: pop.eq {r4,r5,r6,r7,pc}
  00089e30: blx 0x0006e824
```
