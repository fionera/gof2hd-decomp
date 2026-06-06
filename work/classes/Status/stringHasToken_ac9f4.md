# Status::stringHasToken
elf 0xac9f4 body 26
Sig: undefined __thiscall stringHasToken(Status * this, String param_1, String param_2)

## decompile
```c

/* Status::stringHasToken(AbyssEngine::String, AbyssEngine::String) */

bool __thiscall Status::stringHasToken(undefined4 param_1,String *param_2)

{
  uint uVar1;
  
  uVar1 = AbyssEngine::String::IndexOf(param_2);
  return uVar1 < 0x80000000;
}

```
## target disasm
```
  000bc9f4: push {r7,lr}
  000bc9f6: mov r7,sp
  000bc9f8: mov r0,r1
  000bc9fa: mov r1,r2
  000bc9fc: blx 0x0006f3a0
  000bca00: movs r1,#0x0
  000bca02: cmp.w r0,#0xffffffff
  000bca06: it gt
  000bca08: mov.gt r1,#0x1
  000bca0a: mov r0,r1
  000bca0c: pop {r7,pc}
```
