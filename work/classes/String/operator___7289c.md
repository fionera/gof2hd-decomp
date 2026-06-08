# String::operator+=
elf 0x7289c body 98
Sig: undefined __thiscall operator+=(String * this, longlong * param_1)

## decompile
```c

/* AbyssEngine::String::TEMPNAMEPLACEHOLDERVALUE(long long const&) */

void __thiscall AbyssEngine::String::operator+=(String *this,longlong *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  String aSStack_28 [4];
  undefined4 local_24;
  int local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_00082900 + 0x828ae);
  local_1c = *piVar2;
  String(aSStack_28,*param_1);
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
  0008289c: push {r4,r5,r6,r7,lr}
  0008289e: add r7,sp,#0xc
  000828a0: push.w r11
  000828a4: sub sp,#0x10
  000828a6: mov r4,r0
  000828a8: ldr r0,[0x00082900]
  000828aa: add r0,pc
  000828ac: ldr r5,[r0,#0x0]
  000828ae: ldr r0,[r5,#0x0]
  000828b0: str r0,[sp,#0xc]
  000828b2: mov r0,sp
  000828b4: ldrd r2,r3,[r1,#0x0]
  000828b8: blx 0x0006f5bc
  000828bc: ldr r1,[sp,#0x8]
  000828be: cbz r1,0x000828e2
  000828c0: ldrd r0,r6,[r4,#0x4]
  000828c4: add r1,r6
  000828c6: str r1,[r4,#0x8]
  000828c8: movs r2,#0x2
  000828ca: add.w r1,r2,r1, lsl #0x1
  000828ce: blx 0x0006ecc8
  000828d2: str r0,[r4,#0x4]
  000828d4: add.w r0,r0,r6, lsl #0x1
  000828d8: ldrd r1,r2,[sp,#0x4]
  000828dc: lsls r2,r2,#0x1
  000828de: blx 0x0006f1e4
  000828e2: mov r0,sp
  000828e4: blx 0x0006f580
  000828e8: ldr r0,[sp,#0xc]
  000828ea: ldr r1,[r5,#0x0]
  000828ec: subs r0,r1,r0
  000828ee: itttt eq
  000828f0: mov.eq r0,r4
  000828f2: add.eq sp,#0x10
  000828f4: pop.eq.w r11
  000828f8: pop.eq {r4,r5,r6,r7,pc}
  000828fa: blx 0x0006e824
```
