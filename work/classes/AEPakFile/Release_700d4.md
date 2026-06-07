# AEPakFile::Release
elf 0x700d4 body 34
Sig: undefined __thiscall Release(AEPakFile * this)

## decompile
```c

/* AEPakFile::Release() */

undefined4 __thiscall AEPakFile::Release(AEPakFile *this)

{
  if (*(int **)(this + 4) != (int *)0x0) {
    (**(code **)(**(int **)(this + 4) + 0x44))();
    if (*(int **)(this + 4) != (int *)0x0) {
      (**(code **)(**(int **)(this + 4) + 4))();
    }
    *(undefined4 *)(this + 4) = 0;
  }
  return 1;
}

```

## target disasm
```
  000800d4: push {r4,r6,r7,lr}
  000800d6: add r7,sp,#0x8
  000800d8: mov r4,r0
  000800da: ldr r0,[r0,#0x4]
  000800dc: cbz r0,0x000800f2
  000800de: ldr r1,[r0,#0x0]
  000800e0: ldr r1,[r1,#0x44]
  000800e2: blx r1
  000800e4: ldr r0,[r4,#0x4]
  000800e6: cbz r0,0x000800ee
  000800e8: ldr r1,[r0,#0x0]
  000800ea: ldr r1,[r1,#0x4]
  000800ec: blx r1
  000800ee: movs r0,#0x0
  000800f0: str r0,[r4,#0x4]
  000800f2: movs r0,#0x1
  000800f4: pop {r4,r6,r7,pc}
```
