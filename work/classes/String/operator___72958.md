# String::operator+=
elf 0x72958 body 96
Sig: undefined __thiscall operator+=(String * this, float * param_1)

## decompile
```c

/* AbyssEngine::String::TEMPNAMEPLACEHOLDERVALUE(float const&) */

void __thiscall AbyssEngine::String::operator+=(String *this,float *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  float in_s0;
  String aSStack_28 [4];
  undefined4 local_24;
  int local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_000829b8 + 0x8296a);
  local_1c = *piVar2;
  String(aSStack_28,in_s0);
  if (local_20 != 0) {
    iVar3 = *(int *)(this + 8);
    iVar1 = local_20 + iVar3;
    *(int *)(this + 8) = iVar1;
    iVar1 = realloc(*(undefined4 *)(this + 4),iVar1 * 2 + 2);
    *(int *)(this + 4) = iVar1;
    __aeabi_memcpy(iVar1 + iVar3 * 2,local_24,local_20 << 1);
  }
  ~String(aSStack_28);
  iVar1 = *piVar2 - local_1c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  00082958: push {r4,r5,r6,r7,lr}
  0008295a: add r7,sp,#0xc
  0008295c: push.w r11
  00082960: sub sp,#0x10
  00082962: mov r4,r0
  00082964: ldr r0,[0x000829b8]
  00082966: add r0,pc
  00082968: ldr r5,[r0,#0x0]
  0008296a: ldr r0,[r5,#0x0]
  0008296c: str r0,[sp,#0xc]
  0008296e: mov r0,sp
  00082970: ldr r1,[r1,#0x0]
  00082972: blx 0x0006f5d4
  00082976: ldr r1,[sp,#0x8]
  00082978: cbz r1,0x0008299c
  0008297a: ldrd r0,r6,[r4,#0x4]
  0008297e: add r1,r6
  00082980: str r1,[r4,#0x8]
  00082982: movs r2,#0x2
  00082984: add.w r1,r2,r1, lsl #0x1
  00082988: blx 0x0006ecc8
  0008298c: str r0,[r4,#0x4]
  0008298e: add.w r0,r0,r6, lsl #0x1
  00082992: ldrd r1,r2,[sp,#0x4]
  00082996: lsls r2,r2,#0x1
  00082998: blx 0x0006f1e4
  0008299c: mov r0,sp
  0008299e: blx 0x0006f580
  000829a2: ldr r0,[sp,#0xc]
  000829a4: ldr r1,[r5,#0x0]
  000829a6: subs r0,r1,r0
  000829a8: itttt eq
  000829aa: mov.eq r0,r4
  000829ac: add.eq sp,#0x10
  000829ae: pop.eq.w r11
  000829b2: pop.eq {r4,r5,r6,r7,pc}
  000829b4: blx 0x0006e824
```
