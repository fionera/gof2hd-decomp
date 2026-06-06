# AEFile::Write
elf 0x70f50 body 42
Sig: undefined __thiscall Write(AEFile * this, longlong param_1, uint param_2)

## decompile
```c

/* AEFile::Write(long long, unsigned int) */

void __thiscall AEFile::Write(AEFile *this,longlong param_1,uint param_2)

{
  int *piVar1;
  AEFile *local_20 [2];
  uint uStack_18;
  int local_14;
  
  uStack_18 = (uint)param_1;
  piVar1 = *(int **)(DAT_00080f7c + 0x80f5c);
  local_14 = *piVar1;
  local_20[0] = this;
  Write(8,local_20,uStack_18);
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00080f50: push {r0,r1,r2,r3,r4,r6,r7,lr}
  00080f52: add r7,sp,#0x18
  00080f54: ldr r3,[0x00080f7c]
  00080f56: str r1,[sp,#0x4]
  00080f58: add r3,pc
  00080f5a: ldr r4,[r3,#0x0]
  00080f5c: ldr r1,[r4,#0x0]
  00080f5e: str r0,[sp,#0x0]
  00080f60: movs r0,#0x8
  00080f62: str r1,[sp,#0xc]
  00080f64: mov r1,sp
  00080f66: blx 0x0006f40c
  00080f6a: ldr r1,[sp,#0xc]
  00080f6c: ldr r2,[r4,#0x0]
  00080f6e: subs r1,r2,r1
  00080f70: itt eq
  00080f72: add.eq sp,#0x10
  00080f74: pop.eq {r4,r6,r7,pc}
  00080f76: blx 0x0006e824
```
