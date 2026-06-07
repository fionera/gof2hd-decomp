# AENormalFile::Release
elf 0x70140 body 34
Sig: undefined __thiscall Release(AENormalFile * this)

## decompile
```c

/* AENormalFile::Release() */

undefined4 __thiscall AENormalFile::Release(AENormalFile *this)

{
  if (*(int **)(this + 4) != (int *)0x0) {
    (**(code **)(**(int **)(this + 4) + 0x44))();
    if (*(int **)(this + 4) != (int *)0x0) {
      (**(code **)(**(int **)(this + 4) + 4))();
    }
  }
  *(undefined4 *)(this + 4) = 0;
  return 1;
}

```

## target disasm
```
  00080140: push {r4,r6,r7,lr}
  00080142: add r7,sp,#0x8
  00080144: mov r4,r0
  00080146: ldr r0,[r0,#0x4]
  00080148: cbz r0,0x0008015a
  0008014a: ldr r1,[r0,#0x0]
  0008014c: ldr r1,[r1,#0x44]
  0008014e: blx r1
  00080150: ldr r0,[r4,#0x4]
  00080152: cbz r0,0x0008015a
  00080154: ldr r1,[r0,#0x0]
  00080156: ldr r1,[r1,#0x4]
  00080158: blx r1
  0008015a: movs r0,#0x0
  0008015c: str r0,[r4,#0x4]
  0008015e: movs r0,#0x1
  00080160: pop {r4,r6,r7,pc}
```
