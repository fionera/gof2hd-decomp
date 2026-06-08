# String::Set
elf 0x723dc body 168
Sig: undefined __thiscall Set(String * this, ushort * param_1)

## decompile
```c

/* AbyssEngine::String::Set(unsigned short const*) */

void __thiscall AbyssEngine::String::Set(String *this,ushort *param_1)

{
  ushort uVar1;
  ushort *puVar2;
  uint uVar4;
  void *pvVar5;
  undefined2 *puVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  ushort *puVar3;
  
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  puVar2 = param_1;
  if (param_1 == (ushort *)0x0) {
    (*(code *)(DAT_001ab104 + 0x1ab108))(this,DAT_00082484 + 0x82444);
    return;
  }
  do {
    puVar3 = puVar2 + 1;
    uVar1 = *puVar2;
    puVar2 = puVar3;
  } while (uVar1 != 0);
  uVar7 = (int)puVar3 - (int)param_1 >> 1;
  uVar8 = uVar7 - 1;
  *(uint *)(this + 8) = uVar8;
  if (uVar8 == 0) {
    puVar6 = operator_new__(2);
    *(undefined2 **)(this + 4) = puVar6;
    *puVar6 = 0;
  }
  else {
    iVar9 = uVar7 - 2;
    if (param_1[iVar9] == 0) {
      uVar7 = uVar8 * 2;
      if (CARRY4(uVar8,uVar8)) {
        uVar7 = 0xffffffff;
      }
      pvVar5 = operator_new__(uVar7);
      *(void **)(this + 4) = pvVar5;
      __aeabi_memcpy(pvVar5,param_1,uVar8 * 2);
      *(undefined2 *)((int)pvVar5 + iVar9 * 2) = 0;
      *(int *)(this + 8) = iVar9;
    }
    else {
      uVar4 = uVar7 * 2;
      if (CARRY4(uVar7,uVar7)) {
        uVar4 = 0xffffffff;
      }
      pvVar5 = operator_new__(uVar4);
      *(void **)(this + 4) = pvVar5;
      __aeabi_memcpy(pvVar5,param_1,uVar8 * 2);
      *(undefined2 *)((int)pvVar5 + uVar8 * 2) = 0;
    }
  }
  return;
}

```

## target disasm
```
  000823dc: push {r4,r5,r6,r7,lr}
  000823de: add r7,sp,#0xc
  000823e0: push {r8,r9,r11}
  000823e4: mov r4,r0
  000823e6: ldr r0,[r0,#0x4]
  000823e8: mov r5,r1
  000823ea: cbz r0,0x000823f0
  000823ec: blx 0x0006ebfc
  000823f0: movs r0,#0x0
  000823f2: cmp r5,#0x0
  000823f4: str r0,[r4,#0x4]
  000823f6: beq 0x0008243c
  000823f8: mov r0,r5
  000823fa: ldrh.w r1,[r0],#0x2
  000823fe: cmp r1,#0x0
  00082400: bne 0x000823fa
  00082402: subs r0,r0,r5
  00082404: mov.w r1,#0xffffffff
  00082408: add.w r6,r1,r0, asr #0x1
  0008240c: str r6,[r4,#0x8]
  0008240e: cbz r6,0x0008244e
  00082410: asrs r0,r0,#0x1
  00082412: sub.w r9,r0,#0x2
  00082416: ldrh.w r1,[r5,r9,lsl #0x1]
  0008241a: cbz r1,0x0008245c
  0008241c: adds r0,r0,r0
  0008241e: it cs
  00082420: mov.cs.w r0,#0xffffffff
  00082424: blx 0x0006ec08
  00082428: lsls r2,r6,#0x1
  0008242a: mov r1,r5
  0008242c: mov r8,r0
  0008242e: str r0,[r4,#0x4]
  00082430: blx 0x0006f1e4
  00082434: movs r0,#0x0
  00082436: strh.w r0,[r8,r6,lsl #0x1]
  0008243a: b 0x0008247e
  0008243c: ldr r1,[0x00082484]
  0008243e: mov r0,r4
  00082440: add r1,pc
  00082442: pop.w {r8,r9,r11}
  00082446: pop.w {r4,r5,r6,r7,lr}
  0008244a: b.w 0x001ab0f8
  0008244e: movs r0,#0x2
  00082450: blx 0x0006ec08
  00082454: movs r1,#0x0
  00082456: str r0,[r4,#0x4]
  00082458: strh r1,[r0,#0x0]
  0008245a: b 0x0008247e
  0008245c: adds r0,r6,r6
  0008245e: it cs
  00082460: mov.cs.w r0,#0xffffffff
  00082464: blx 0x0006ec08
  00082468: lsls r2,r6,#0x1
  0008246a: mov r1,r5
  0008246c: mov r8,r0
  0008246e: str r0,[r4,#0x4]
  00082470: blx 0x0006f1e4
  00082474: movs r0,#0x0
  00082476: strh.w r0,[r8,r9,lsl #0x1]
  0008247a: str.w r9,[r4,#0x8]
  0008247e: pop.w {r8,r9,r11}
  00082482: pop {r4,r5,r6,r7,pc}
```
