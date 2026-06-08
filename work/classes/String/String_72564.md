# String::String
elf 0x72564 body 42
Sig: undefined __thiscall String(String * this, String * param_1, bool param_2)

## decompile
```c

/* AbyssEngine::String::String(AbyssEngine::String const&, bool) */

String * __thiscall AbyssEngine::String::String(String *this,String *param_1,bool param_2)

{
  *(int *)this = *(int *)(DAT_00082590 + 0x82574) + 8;
  *(undefined4 *)(this + 4) = 0;
  Set(this,*(ushort **)(param_1 + 4));
  if (param_2) {
    Reverse(this);
  }
  return this;
}

```

## target disasm
```
  00082564: push {r4,r5,r7,lr}
  00082566: add r7,sp,#0x8
  00082568: mov r4,r0
  0008256a: ldr r0,[0x00082590]
  0008256c: mov r5,r2
  0008256e: movs r2,#0x0
  00082570: add r0,pc
  00082572: ldr r0,[r0,#0x0]
  00082574: adds r0,#0x8
  00082576: strd r0,r2,[r4,#0x0]
  0008257a: ldr r1,[r1,#0x4]
  0008257c: mov r0,r4
  0008257e: blx 0x0006f3f4
  00082582: cbz r5,0x0008258a
  00082584: mov r0,r4
  00082586: blx 0x0006f58c
  0008258a: mov r0,r4
  0008258c: pop {r4,r5,r7,pc}
```
