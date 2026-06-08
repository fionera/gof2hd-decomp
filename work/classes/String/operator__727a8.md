# String::operator=
elf 0x727a8 body 20
Sig: undefined __thiscall operator=(String * this, String * param_1)

## decompile
```c

/* AbyssEngine::String::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&) */

String * __thiscall AbyssEngine::String::operator=(String *this,String *param_1)

{
  if (*(ushort **)(param_1 + 4) != (ushort *)0x0) {
    Set(this,*(ushort **)(param_1 + 4));
  }
  return this;
}

```

## target disasm
```
  000827a8: push {r4,r6,r7,lr}
  000827aa: add r7,sp,#0x8
  000827ac: ldr r1,[r1,#0x4]
  000827ae: mov r4,r0
  000827b0: cbz r1,0x000827b8
  000827b2: mov r0,r4
  000827b4: blx 0x0006f3f4
  000827b8: mov r0,r4
  000827ba: pop {r4,r6,r7,pc}
```
