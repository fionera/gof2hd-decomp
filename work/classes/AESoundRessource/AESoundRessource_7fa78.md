# AESoundRessource::AESoundRessource
elf 0x7fa78 body 22
Sig: undefined __thiscall AESoundRessource(AESoundRessource * this)

## decompile
```c

/* AbyssEngine::AESoundRessource::AESoundRessource() */

AESoundRessource * __thiscall
AbyssEngine::AESoundRessource::AESoundRessource(AESoundRessource *this)

{
  Array<AbyssEngine::AESoundInterface*>::Array((Array<AbyssEngine::AESoundInterface*> *)(this + 4));
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  0008fa78: push {r4,r6,r7,lr}
  0008fa7a: add r7,sp,#0x8
  0008fa7c: mov r4,r0
  0008fa7e: adds r0,#0x4
  0008fa80: blx 0x00070084
  0008fa84: movs r0,#0x0
  0008fa86: str r0,[r4,#0x10]
  0008fa88: str r0,[r4,#0x0]
  0008fa8a: mov r0,r4
  0008fa8c: pop {r4,r6,r7,pc}
```
