# MeshMerger::setMesh
elf 0x173bf8 body 78
Sig: undefined __thiscall setMesh(MeshMerger * this, int param_1, signed param_2, ushort param_3)

## decompile
```c

/* MeshMerger::setMesh(int, signed char, unsigned short) */

void __thiscall MeshMerger::setMesh(MeshMerger *this,int param_1,int param_3,ushort param_4)

{
  undefined4 uVar1;
  int *piVar2;
  uint local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_00183c48 + 0x183c0e);
  local_1c = *piVar2;
  AbyssEngine::PaintCanvas::MeshCreate(*(PaintCanvas **)(this + 0xc),param_4,&local_20,false);
  uVar1 = AbyssEngine::PaintCanvas::MeshGetPointer(*(PaintCanvas **)(this + 0xc),local_20);
  *(undefined4 *)(*(int *)(this + 8) + (*(int *)this * param_3 + param_1) * 4) = uVar1;
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00183bf8: push {r4,r5,r6,r7,lr}
  00183bfa: add r7,sp,#0xc
  00183bfc: push.w r8
  00183c00: sub sp,#0x8
  00183c02: mov r6,r0
  00183c04: ldr r0,[0x00183c48]
  00183c06: mov r8,r2
  00183c08: mov r5,r1
  00183c0a: add r0,pc
  00183c0c: mov r1,r3
  00183c0e: mov r2,sp
  00183c10: movs r3,#0x0
  00183c12: ldr r4,[r0,#0x0]
  00183c14: ldr r0,[r4,#0x0]
  00183c16: str r0,[sp,#0x4]
  00183c18: ldr r0,[r6,#0xc]
  00183c1a: blx 0x0006fcf4
  00183c1e: ldr r0,[r6,#0xc]
  00183c20: ldr r1,[sp,#0x0]
  00183c22: blx 0x00072370
  00183c26: ldr r1,[r6,#0x0]
  00183c28: ldr r2,[r6,#0x8]
  00183c2a: mla r1,r1,r8,r5
  00183c2e: str.w r0,[r2,r1,lsl #0x2]
  00183c32: ldr r0,[sp,#0x4]
  00183c34: ldr r1,[r4,#0x0]
  00183c36: subs r0,r1,r0
  00183c38: ittt eq
  00183c3a: add.eq sp,#0x8
  00183c3c: pop.eq.w r8
  00183c40: pop.eq {r4,r5,r6,r7,pc}
  00183c42: blx 0x0006e824
```
