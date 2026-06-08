# String::String
elf 0x723b0 body 40
Sig: undefined __thiscall String(String * this, ushort * param_1, bool param_2)

## decompile
```c

/* AbyssEngine::String::String(unsigned short const*, bool) */

String * __thiscall AbyssEngine::String::String(String *this,ushort *param_1,bool param_2)

{
  *(int *)this = *(int *)(DAT_000823d8 + 0x823c0) + 8;
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
  000823b0: push {r4,r5,r7,lr}
  000823b2: add r7,sp,#0x8
  000823b4: mov r4,r0
  000823b6: ldr r0,[0x000823d8]
  000823b8: mov r5,r2
  000823ba: movs r2,#0x0
  000823bc: add r0,pc
  000823be: ldr r0,[r0,#0x0]
  000823c0: adds r0,#0x8
  000823c2: strd r0,r2,[r4,#0x0]
  000823c6: mov r0,r4
  000823c8: blx 0x0006f3f4
  000823cc: cbz r5,0x000823d4
  000823ce: mov r0,r4
  000823d0: blx 0x0006f58c
  000823d4: mov r0,r4
  000823d6: pop {r4,r5,r7,pc}
```
