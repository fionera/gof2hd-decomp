# CheatCode::CheatCode
elf 0x74378 body 30
Sig: undefined __thiscall CheatCode(CheatCode * this)

## decompile
```c

/* AbyssEngine::CheatCode::CheatCode() */

CheatCode * __thiscall AbyssEngine::CheatCode::CheatCode(CheatCode *this)

{
  Array<unsigned_short> *this_00;
  
  this_00 = operator_new(0xc);
  Array<unsigned_short>::Array(this_00);
  *(Array<unsigned_short> **)this = this_00;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  return this;
}

```

## target disasm
```
  00084378: push {r4,r5,r7,lr}
  0008437a: add r7,sp,#0x8
  0008437c: mov r4,r0
  0008437e: movs r0,#0xc
  00084380: blx 0x0006eb24
  00084384: mov r5,r0
  00084386: blx 0x0006f6ac
  0008438a: movs r0,#0x0
  0008438c: strd r5,r0,[r4,#0x0]
  00084390: str r0,[r4,#0x8]
  00084392: mov r0,r4
  00084394: pop {r4,r5,r7,pc}
```
