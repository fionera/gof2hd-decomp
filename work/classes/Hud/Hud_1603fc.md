# Hud::Hud
elf 0x1603fc body 100
Sig: undefined __thiscall Hud(Hud * this)

## decompile
```c

/* Hud::Hud() */

Hud * __thiscall Hud::Hud(Hud *this)

{
  EVP_PKEY_CTX *ctx;
  int iVar1;
  
  iVar1 = 0x1c;
  do {
    AbyssEngine::String::String((String *)(this + iVar1));
    iVar1 = iVar1 + 0xc;
  } while (iVar1 != 0x10c);
  AbyssEngine::String::String((String *)(this + 0x1e0));
  AbyssEngine::String::String((String *)(this + 500));
  AbyssEngine::String::String((String *)(this + 0x200));
  AbyssEngine::String::String((String *)(this + 0x228));
  AbyssEngine::String::String((String *)(this + 0x3b4));
  AbyssEngine::String::String((String *)(this + 0x51c));
  init(this,ctx);
  return this;
}

```

## target disasm
```
  001703fc: push {r4,r5,r6,r7,lr}
  001703fe: add r7,sp,#0xc
  00170400: push {r7,r8,r9,r10,r11}
  00170404: mov r4,r0
  00170406: movs r5,#0x1c
  00170408: adds r0,r4,r5
  0017040a: blx 0x0006efbc
  0017040e: adds r5,#0xc
  00170410: cmp.w r5,#0x10c
  00170414: bne 0x00170408
  00170416: add.w r10,r4,#0x1e0
  0017041a: mov r0,r10
  0017041c: blx 0x0006efbc
  00170420: add.w r9,r4,#0x1f4
  00170424: mov r0,r9
  00170426: blx 0x0006efbc
  0017042a: add.w r11,r4,#0x200
  0017042e: mov r0,r11
  00170430: blx 0x0006efbc
  00170434: add.w r6,r4,#0x228
  00170438: mov r0,r6
  0017043a: blx 0x0006efbc
  0017043e: add.w r5,r4,#0x3b4
  00170442: mov r0,r5
  00170444: blx 0x0006efbc
  00170448: addw r8,r4,#0x51c
  0017044c: mov r0,r8
  0017044e: blx 0x0006efbc
  00170452: mov r0,r4
  00170454: blx 0x000779d4
  00170458: mov r0,r4
  0017045a: pop.w {r3,r8,r9,r10,r11}
  0017045e: pop {r4,r5,r6,r7,pc}
```
