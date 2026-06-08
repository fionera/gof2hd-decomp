# AbyssEngine::operator+
elf 0x73654 body 12
Sig: undefined __thiscall operator+(AbyssEngine * this, String * param_1)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,String *param_1)

{
  String::String((String *)this,param_1,false);
  return;
}

```

## target disasm
```
  00083654: push {r7,lr}
  00083656: mov r7,sp
  00083658: movs r2,#0x0
  0008365a: blx 0x0006f5b0
  0008365e: pop {r7,pc}
```
