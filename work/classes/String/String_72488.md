# String::String
elf 0x72488 body 40
Sig: undefined __thiscall String(String * this, char * param_1, bool param_2)

## decompile
```c

/* AbyssEngine::String::String(char const*, bool) */

String * __thiscall AbyssEngine::String::String(String *this,char *param_1,bool param_2)

{
  *(int *)this = *(int *)(DAT_000824b0 + 0x82498) + 8;
  *(undefined4 *)(this + 4) = 0;
  Set(this,param_1);
  if (param_2) {
    Reverse(this);
  }
  return this;
}

```

## target disasm
```
  00082488: push {r4,r5,r7,lr}
  0008248a: add r7,sp,#0x8
  0008248c: mov r4,r0
  0008248e: ldr r0,[0x000824b0]
  00082490: mov r5,r2
  00082492: movs r2,#0x0
  00082494: add r0,pc
  00082496: ldr r0,[r0,#0x0]
  00082498: adds r0,#0x8
  0008249a: strd r0,r2,[r4,#0x0]
  0008249e: mov r0,r4
  000824a0: blx 0x0006f2bc
  000824a4: cbz r5,0x000824ac
  000824a6: mov r0,r4
  000824a8: blx 0x0006f58c
  000824ac: mov r0,r4
  000824ae: pop {r4,r5,r7,pc}
```
