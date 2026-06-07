# ConfigReader::~ConfigReader
elf 0x87672 body 74
Sig: undefined __thiscall ~ConfigReader(ConfigReader * this)

## decompile
```c

/* AbyssEngine::ConfigReader::~ConfigReader() */

void __thiscall AbyssEngine::ConfigReader::~ConfigReader(ConfigReader *this)

{
  String *this_00;
  void *pvVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  
  iVar4 = 0;
  for (uVar3 = 0; uVar3 < *(uint *)this; uVar3 = uVar3 + 1) {
    this_00 = *(String **)(*(int *)(this + 4) + iVar4);
    if (this_00 == (String *)0x0) {
      puVar2 = (undefined4 *)(*(int *)(this + 4) + uVar3 * 4);
    }
    else {
      pvVar1 = (void *)String::~String(this_00);
      operator_delete(pvVar1);
      puVar2 = (undefined4 *)(*(int *)(this + 4) + iVar4);
    }
    *puVar2 = 0;
    iVar4 = iVar4 + 4;
  }
  (*(code *)(DAT_001ab764 + 0x1ab768))(this);
  return;
}

```

## target disasm
```
  00097672: push {r4,r5,r6,r7,lr}
  00097674: add r7,sp,#0xc
  00097676: push.w r8
  0009767a: mov r4,r0
  0009767c: mov.w r8,#0x0
  00097680: movs r6,#0x0
  00097682: movs r5,#0x0
  00097684: b 0x000976a6
  00097686: ldr r1,[r4,#0x4]
  00097688: ldr r0,[r1,r6]
  0009768a: cbz r0,0x0009769a
  0009768c: blx 0x0006ee30
  00097690: blx 0x0006eb48
  00097694: ldr r0,[r4,#0x4]
  00097696: add r0,r6
  00097698: b 0x0009769e
  0009769a: add.w r0,r1,r5, lsl #0x2
  0009769e: str.w r8,[r0,#0x0]
  000976a2: adds r6,#0x4
  000976a4: adds r5,#0x1
  000976a6: ldr r0,[r4,#0x0]
  000976a8: cmp r5,r0
  000976aa: bcc 0x00097686
  000976ac: mov r0,r4
  000976ae: pop.w r8
  000976b2: pop.w {r4,r5,r6,r7,lr}
  000976b6: b.w 0x001ab758
  001ab758: bx pc
```
