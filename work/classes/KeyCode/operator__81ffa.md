# KeyCode::operator=
elf 0x81ffa body 18
Sig: undefined __thiscall operator=(KeyCode * this, KeyCode * param_1)

## decompile
```c

/* AbyssEngine::KeyCode::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::KeyCode const&) */

KeyCode * __thiscall AbyssEngine::KeyCode::operator=(KeyCode *this,KeyCode *param_1)

{
  *(undefined4 *)this = *(undefined4 *)param_1;
  String::operator=((String *)(this + 4),param_1 + 4);
  return this;
}

```

## target disasm
```
  00091ffa: push {r4,r6,r7,lr}
  00091ffc: add r7,sp,#0x8
  00091ffe: ldmia r1!,{r2}
  00092000: mov r4,r0
  00092002: stmia r0!,{r2}
  00092004: blx 0x0006f2b0
  00092008: mov r0,r4
  0009200a: pop {r4,r6,r7,pc}
```
