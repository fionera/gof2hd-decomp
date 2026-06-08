# String::StrLen
elf 0x729f0 body 28
Sig: undefined __thiscall StrLen(String * this, ushort * param_1)

## decompile
```c

/* AbyssEngine::String::StrLen(unsigned short const*) */

int __thiscall AbyssEngine::String::StrLen(String *this,ushort *param_1)

{
  ushort uVar1;
  ushort *puVar2;
  ushort *puVar3;
  
  puVar2 = param_1;
  if (param_1 == (ushort *)0x0) {
    return 0;
  }
  do {
    puVar3 = puVar2 + 1;
    uVar1 = *puVar2;
    puVar2 = puVar3;
  } while (uVar1 != 0);
  return ((int)puVar3 - (int)param_1 >> 1) + -1;
}

```

## target disasm
```
  000829f0: cbz r1,0x00082a08
  000829f2: mov r0,r1
  000829f4: ldrh.w r2,[r0],#0x2
  000829f8: cmp r2,#0x0
  000829fa: bne 0x000829f4
  000829fc: subs r0,r0,r1
  000829fe: mov.w r1,#0xffffffff
  00082a02: add.w r0,r1,r0, asr #0x1
  00082a06: bx lr
  00082a08: movs r0,#0x0
  00082a0a: bx lr
```
