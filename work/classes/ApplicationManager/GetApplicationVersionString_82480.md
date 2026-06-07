# ApplicationManager::GetApplicationVersionString
elf 0x82480 body 16
Sig: undefined __thiscall GetApplicationVersionString(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetApplicationVersionString() */

void __thiscall
AbyssEngine::ApplicationManager::GetApplicationVersionString(ApplicationManager *this)

{
  String::String((String *)this,(char *)(DAT_00092490 + 0x9248c),false);
  return;
}

```

## target disasm
```
  00092480: push {r7,lr}
  00092482: mov r7,sp
  00092484: ldr r1,[0x00092490]
  00092486: movs r2,#0x0
  00092488: add r1,pc
  0009248a: blx 0x0006ee18
  0009248e: pop {r7,pc}
```
