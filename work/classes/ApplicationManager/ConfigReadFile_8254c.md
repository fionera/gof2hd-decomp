# ApplicationManager::ConfigReadFile
elf 0x8254c body 68
Sig: undefined __thiscall ConfigReadFile(ApplicationManager * this, String param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::ConfigReadFile(AbyssEngine::String) */

void __thiscall
AbyssEngine::ApplicationManager::ConfigReadFile(ApplicationManager *this,String *param_2)

{
  ConfigReader *pCVar1;
  int *piVar2;
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000925a0 + 0x9255c);
  local_1c = *piVar2;
  pCVar1 = *(ConfigReader **)(this + 0x38);
  if (pCVar1 != (ConfigReader *)0x0) {
    String::String(aSStack_28,param_2,false);
    ConfigReader::ParseFile(pCVar1,aSStack_28);
    String::~String(aSStack_28);
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0009254c: push {r4,r5,r6,r7,lr}
  0009254e: add r7,sp,#0xc
  00092550: push.w r11
  00092554: sub sp,#0x10
  00092556: ldr r2,[0x000925a0]
  00092558: add r2,pc
  0009255a: ldr r6,[r2,#0x0]
  0009255c: ldr r2,[r6,#0x0]
  0009255e: str r2,[sp,#0xc]
  00092560: ldr r4,[r0,#0x38]
  00092562: cbz r4,0x0009257c
  00092564: mov r5,sp
  00092566: movs r2,#0x0
  00092568: mov r0,r5
  0009256a: blx 0x0006f028
  0009256e: mov r0,r4
  00092570: mov r1,r5
  00092572: blx 0x00070408
  00092576: mov r0,sp
  00092578: blx 0x0006ee30
  0009257c: ldr r0,[sp,#0xc]
  0009257e: ldr r1,[r6,#0x0]
  00092580: subs r0,r1,r0
  00092582: ittt eq
  00092584: add.eq sp,#0x10
  00092586: pop.eq.w r11
  0009258a: pop.eq {r4,r5,r6,r7,pc}
  0009258c: blx 0x0006e824
```
