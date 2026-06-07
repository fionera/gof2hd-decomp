# Engine::initFileInterface
elf 0x6daf4 body 32
Sig: undefined __thiscall initFileInterface(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::initFileInterface() */

void __thiscall AbyssEngine::Engine::initFileInterface(Engine *this)

{
  FileInterfaceAndroid *this_00;
  
  this_00 = operator_new(0x38);
  FileInterfaceAndroid::FileInterfaceAndroid(this_00);
  *(FileInterfaceAndroid **)(this + 0x24) = this_00;
  (*(code *)(DAT_001ab044 + 0x1ab048))(this_00);
  return;
}

```

## target disasm
```
  0007daf4: push {r4,r5,r7,lr}
  0007daf6: add r7,sp,#0x8
  0007daf8: mov r4,r0
  0007dafa: movs r0,#0x38
  0007dafc: blx 0x0006eb24
  0007db00: mov r5,r0
  0007db02: blx 0x0006ed7c
  0007db06: str r5,[r4,#0x24]
  0007db08: mov r0,r5
  0007db0a: pop.w {r4,r5,r7,lr}
  0007db0e: b.w 0x001ab038
  001ab038: bx pc
```
