# String::ValueOf
elf 0x727bc body 26
Sig: undefined __stdcall ValueOf(void)

## decompile
```c

/* AbyssEngine::String::ValueOf() */

undefined4 AbyssEngine::String::ValueOf(void)

{
  void *pvVar1;
  undefined4 uVar2;
  
  pvVar1 = (void *)GetAEChar();
  uVar2 = atoi();
  operator_delete__(pvVar1);
  return uVar2;
}

```

## target disasm
```
  000827bc: push {r4,r5,r7,lr}
  000827be: add r7,sp,#0x8
  000827c0: blx 0x0006ef68
  000827c4: mov r4,r0
  000827c6: blx 0x0006f5a4
  000827ca: mov r5,r0
  000827cc: mov r0,r4
  000827ce: blx 0x0006ebfc
  000827d2: mov r0,r5
  000827d4: pop {r4,r5,r7,pc}
```
