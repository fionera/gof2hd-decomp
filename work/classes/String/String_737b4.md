# String::String
elf 0x737b4 body 34
Sig: undefined __thiscall String(String * this, int param_1)

## decompile
```c

/* AbyssEngine::String::String(int) */

String * __thiscall AbyssEngine::String::String(String *this,int param_1)

{
  *(int *)this = *(int *)(DAT_000837d8 + 0x837c4) + 8;
  *(undefined4 *)(this + 4) = 0;
  Set(CONCAT44(param_1,this));
  return this;
}

```

## target disasm
```
  000837b4: push {r4,r6,r7,lr}
  000837b6: add r7,sp,#0x8
  000837b8: mov r4,r0
  000837ba: ldr r0,[0x000837d8]
  000837bc: movs r2,#0x0
  000837be: asrs r3,r1,#0x1f
  000837c0: add r0,pc
  000837c2: ldr r0,[r0,#0x0]
  000837c4: adds r0,#0x8
  000837c6: strd r0,r2,[r4,#0x0]
  000837ca: mov r0,r4
  000837cc: mov r2,r1
  000837ce: blx 0x0006efd4
  000837d2: mov r0,r4
  000837d4: pop {r4,r6,r7,pc}
```
