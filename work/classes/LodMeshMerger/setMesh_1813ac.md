# LodMeshMerger::setMesh
elf 0x1813ac body 78
Sig: undefined __thiscall setMesh(LodMeshMerger * this, int param_1, signed param_2, ushort param_3)

## decompile
```c

/* LodMeshMerger::setMesh(int, signed char, unsigned short) */

void __thiscall LodMeshMerger::setMesh(LodMeshMerger *this,int param_1,int param_3,ushort param_4)

{
  undefined4 uVar1;
  int *piVar2;
  uint local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_001913fc + 0x1913c2);
  local_1c = *piVar2;
  AbyssEngine::PaintCanvas::MeshCreate(*(PaintCanvas **)(this + 0x14),param_4,&local_20,false);
  uVar1 = AbyssEngine::PaintCanvas::MeshGetPointer(*(PaintCanvas **)(this + 0x14),local_20);
  *(undefined4 *)(*(int *)(this + 0xc) + (*(int *)this * param_3 + param_1) * 4) = uVar1;
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001913ac: push {r4,r5,r6,r7,lr}
  001913ae: add r7,sp,#0xc
  001913b0: push.w r8
  001913b4: sub sp,#0x8
  001913b6: mov r6,r0
  001913b8: ldr r0,[0x001913fc]
  001913ba: mov r8,r2
  001913bc: mov r5,r1
  001913be: add r0,pc
  001913c0: mov r1,r3
  001913c2: mov r2,sp
  001913c4: movs r3,#0x0
  001913c6: ldr r4,[r0,#0x0]
  001913c8: ldr r0,[r4,#0x0]
  001913ca: str r0,[sp,#0x4]
  001913cc: ldr r0,[r6,#0x14]
  001913ce: blx 0x0006fcf4
  001913d2: ldr r0,[r6,#0x14]
  001913d4: ldr r1,[sp,#0x0]
  001913d6: blx 0x00072370
  001913da: ldr r1,[r6,#0x0]
  001913dc: ldr r2,[r6,#0xc]
  001913de: mla r1,r1,r8,r5
  001913e2: str.w r0,[r2,r1,lsl #0x2]
  001913e6: ldr r0,[sp,#0x4]
  001913e8: ldr r1,[r4,#0x0]
  001913ea: subs r0,r1,r0
  001913ec: ittt eq
  001913ee: add.eq sp,#0x8
  001913f0: pop.eq.w r8
  001913f4: pop.eq {r4,r5,r6,r7,pc}
  001913f6: blx 0x0006e824
```
