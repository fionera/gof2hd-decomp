# String::String
elf 0x6e484 body 34
Sig: undefined __thiscall String(String * this, char param_1)

## decompile
```c

/* AbyssEngine::String::String(char) */

String * __thiscall AbyssEngine::String::String(String *this,char param_1)

{
  *(int *)this = *(int *)(DAT_0007e4a8 + 0x7e494) + 8;
  *(undefined4 *)(this + 4) = 0;
  Set(CONCAT44((int)param_1,this));
  return this;
}

```

## target disasm
```
  0007e484: push {r4,r6,r7,lr}
  0007e486: add r7,sp,#0x8
  0007e488: mov r4,r0
  0007e48a: ldr r0,[0x0007e4a8]
  0007e48c: movs r2,#0x0
  0007e48e: movs r3,#0x0
  0007e490: add r0,pc
  0007e492: ldr r0,[r0,#0x0]
  0007e494: adds r0,#0x8
  0007e496: strd r0,r2,[r4,#0x0]
  0007e49a: mov r0,r4
  0007e49c: mov r2,r1
  0007e49e: blx 0x0006efd4
  0007e4a2: mov r0,r4
  0007e4a4: pop {r4,r6,r7,pc}
```
