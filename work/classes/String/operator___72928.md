# String::operator+=
elf 0x72928 body 44
Sig: undefined __thiscall operator+=(String * this, int * param_1)

## decompile
```c

/* AbyssEngine::String::TEMPNAMEPLACEHOLDERVALUE(int const&) */

void __thiscall AbyssEngine::String::operator+=(String *this,int *param_1)

{
  int *piVar1;
  int local_20;
  int local_1c;
  int local_14;
  
  piVar1 = *(int **)(DAT_00082954 + 0x82932);
  local_14 = *piVar1;
  local_20 = *param_1;
  local_1c = local_20 >> 0x1f;
  operator+=(this,(longlong *)&local_20);
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00082928: push {r0,r1,r2,r3,r4,r6,r7,lr}
  0008292a: add r7,sp,#0x18
  0008292c: ldr r2,[0x00082954]
  0008292e: add r2,pc
  00082930: ldr r4,[r2,#0x0]
  00082932: ldr r2,[r4,#0x0]
  00082934: str r2,[sp,#0xc]
  00082936: ldr r1,[r1,#0x0]
  00082938: str r1,[sp,#0x0]
  0008293a: asrs r1,r1,#0x1f
  0008293c: str r1,[sp,#0x4]
  0008293e: mov r1,sp
  00082940: blx 0x0006f5c8
  00082944: ldr r1,[sp,#0xc]
  00082946: ldr r2,[r4,#0x0]
  00082948: subs r1,r2,r1
  0008294a: itt eq
  0008294c: add.eq sp,#0x10
  0008294e: pop.eq {r4,r6,r7,pc}
  00082950: blx 0x0006e824
```
