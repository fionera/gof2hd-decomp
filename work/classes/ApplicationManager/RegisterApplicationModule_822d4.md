# ApplicationManager::RegisterApplicationModule
elf 0x822d4 body 60
Sig: undefined __thiscall RegisterApplicationModule(ApplicationManager * this, uint param_1, IApplicationModule * param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::RegisterApplicationModule(unsigned int,
   AbyssEngine::IApplicationModule*) */

void __thiscall
AbyssEngine::ApplicationManager::RegisterApplicationModule
          (ApplicationManager *this,uint param_1,IApplicationModule *param_2)

{
  if (param_2 == (IApplicationModule *)0x0) {
    return;
  }
  IApplicationModule::SetApplicationManager(param_2,this);
  ArrayAdd<AbyssEngine::IApplicationModule*>(param_2,(Array *)(this + 0x44));
  (*(code *)(DAT_001ab5c4 + 0x1ab5c8))(param_1,this + 0x50);
  return;
}

```

## target disasm
```
  000922d4: push {r4,r5,r6,r7,lr}
  000922d6: add r7,sp,#0xc
  000922d8: push.w r11
  000922dc: mov r6,r2
  000922de: mov r4,r1
  000922e0: mov r5,r0
  000922e2: cbz r2,0x00092308
  000922e4: mov r0,r6
  000922e6: mov r1,r5
  000922e8: blx 0x000703b4
  000922ec: add.w r1,r5,#0x44
  000922f0: mov r0,r6
  000922f2: blx 0x000703c0
  000922f6: add.w r1,r5,#0x50
  000922fa: mov r0,r4
  000922fc: pop.w r11
  00092300: pop.w {r4,r5,r6,r7,lr}
  00092304: b.w 0x001ab5b8
  00092308: pop.w r11
  0009230c: pop {r4,r5,r6,r7,pc}
  001ab5b8: bx pc
```
