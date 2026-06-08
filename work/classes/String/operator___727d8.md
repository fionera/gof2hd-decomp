# String::operator+=
elf 0x727d8 body 116
Sig: undefined __thiscall operator+=(String * this, String * param_1)

## decompile
```c

/* AbyssEngine::String::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&) */

void __thiscall AbyssEngine::String::operator+=(String *this,String *param_1)

{
  int iVar1;
  int iVar2;
  undefined4 unaff_r5;
  int iVar3;
  int unaff_r6;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_0008284c + 0x827e8);
  iVar1 = *piVar4;
  if (*(int *)(param_1 + 8) != 0) {
    String((String *)&stack0xffffffd0,param_1,false);
    iVar3 = *(int *)(this + 8);
    iVar2 = unaff_r6 + iVar3;
    *(int *)(this + 8) = iVar2;
    iVar2 = realloc(*(undefined4 *)(this + 4),iVar2 * 2 + 2);
    *(int *)(this + 4) = iVar2;
    __aeabi_memcpy(iVar2 + iVar3 * 2,unaff_r5,unaff_r6 << 1);
    *(undefined2 *)(iVar2 + *(int *)(this + 8) * 2) = 0;
    ~String((String *)&stack0xffffffd0);
  }
  iVar1 = *piVar4 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  000827d8: push {r4,r5,r6,r7,lr}
  000827da: add r7,sp,#0xc
  000827dc: push {r4,r5,r6,r7,r8,r9,r10}
  000827e0: mov r4,r0
  000827e2: ldr r0,[0x0008284c]
  000827e4: add r0,pc
  000827e6: ldr.w r10,[r0,#0x0]
  000827ea: ldr.w r0,[r10,#0x0]
  000827ee: str r0,[sp,#0xc]
  000827f0: ldr r0,[r1,#0x8]
  000827f2: cbz r0,0x00082834
  000827f4: mov r8,sp
  000827f6: movs r2,#0x0
  000827f8: mov r0,r8
  000827fa: mov.w r9,#0x0
  000827fe: blx 0x0006f5b0
  00082802: ldrd r0,r5,[r4,#0x4]
  00082806: movs r2,#0x2
  00082808: ldr r1,[sp,#0x8]
  0008280a: add r1,r5
  0008280c: str r1,[r4,#0x8]
  0008280e: add.w r1,r2,r1, lsl #0x1
  00082812: blx 0x0006ecc8
  00082816: str r0,[r4,#0x4]
  00082818: mov r6,r0
  0008281a: ldrd r1,r2,[sp,#0x4]
  0008281e: add.w r0,r0,r5, lsl #0x1
  00082822: lsls r2,r2,#0x1
  00082824: blx 0x0006f1e4
  00082828: ldr r0,[r4,#0x8]
  0008282a: strh.w r9,[r6,r0,lsl #0x1]
  0008282e: mov r0,r8
  00082830: blx 0x0006f580
  00082834: ldr r0,[sp,#0xc]
  00082836: ldr.w r1,[r10,#0x0]
  0008283a: subs r0,r1,r0
  0008283c: itttt eq
  0008283e: mov.eq r0,r4
  00082840: add.eq sp,#0x10
  00082842: pop.eq.w {r8,r9,r10}
  00082846: pop.eq {r4,r5,r6,r7,pc}
  00082848: blx 0x0006e824
```
