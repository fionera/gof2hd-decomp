# Globals::getRandomName
elf 0xe40b4 body 298
Sig: undefined __stdcall getRandomName(int param_1, bool param_2)

## decompile
```c

/* Globals::getRandomName(int, bool) */

void Globals::getRandomName(int param_1,bool param_2)

{
  FileRead *this;
  Array *this_00;
  Array *this_01;
  int iVar1;
  void *pvVar2;
  int in_r2;
  bool in_r3;
  int *piVar3;
  String aSStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [8];
  int local_34;
  String aSStack_30 [12];
  int local_24;
  
  piVar3 = *(int **)(DAT_000f4218 + 0xf40ca);
  local_24 = *piVar3;
  this = operator_new(1);
  FileRead::FileRead(this);
  this_00 = (Array *)FileRead::loadNamesBinary(this,in_r2,in_r3,true);
  this_01 = (Array *)FileRead::loadNamesBinary(this,in_r2,in_r3,false);
  if (this_00 == (Array *)0x0) {
    AbyssEngine::String::String(aSStack_30,(char *)(DAT_000f421c + 0xf4124),false);
  }
  else {
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f4220 + 0xf4104));
    AbyssEngine::String::String(aSStack_30,*(String **)(*(int *)(this_00 + 4) + iVar1 * 4),false);
  }
  if (this_01 == (Array *)0x0) {
    AbyssEngine::String::String(aSStack_3c,(char *)(DAT_000f4224 + 0xf414c),false);
  }
  else {
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f4228 + 0xf4130));
    AbyssEngine::String::String(aSStack_3c,*(String **)(*(int *)(this_01 + 4) + iVar1 * 4),false);
  }
  if (this_00 != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(this_00);
    pvVar2 = (void *)Array<AbyssEngine::String*>::~Array((Array<AbyssEngine::String*> *)this_00);
    operator_delete(pvVar2);
  }
  if (this_01 != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(this_01);
    pvVar2 = (void *)Array<AbyssEngine::String*>::~Array((Array<AbyssEngine::String*> *)this_01);
    operator_delete(pvVar2);
  }
  pvVar2 = (void *)FileRead::~FileRead(this);
  operator_delete(pvVar2);
  if (local_34 == 0) {
    AbyssEngine::String::String((String *)param_1,aSStack_30,false);
  }
  else {
    AbyssEngine::String::String(aSStack_54,(char *)(DAT_000f422c + 0xf418a),false);
    AbyssEngine::operator+(aSStack_48,aSStack_30);
    AbyssEngine::operator+((String *)param_1,aSStack_48);
    AbyssEngine::String::~String(aSStack_48);
    AbyssEngine::String::~String(aSStack_54);
  }
  AbyssEngine::String::~String(aSStack_3c);
  AbyssEngine::String::~String(aSStack_30);
  if (*piVar3 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f40b4: push {r4,r5,r6,r7,lr}
  000f40b6: add r7,sp,#0xc
  000f40b8: push {r8,r9,r10}
  000f40bc: sub sp,#0x38
  000f40be: mov r8,r0
  000f40c0: ldr r0,[0x000f4218]
  000f40c2: mov r6,r3
  000f40c4: mov r5,r2
  000f40c6: add r0,pc
  000f40c8: ldr.w r10,[r0,#0x0]
  000f40cc: ldr.w r0,[r10,#0x0]
  000f40d0: str r0,[sp,#0x34]
  000f40d2: movs r0,#0x1
  000f40d4: blx 0x0006eb24
  000f40d8: mov r9,r0
  000f40da: blx 0x00072124
  000f40de: mov r0,r9
  000f40e0: mov r1,r5
  000f40e2: mov r2,r6
  000f40e4: movs r3,#0x1
  000f40e6: blx 0x00075db4
  000f40ea: mov r4,r0
  000f40ec: mov r0,r9
  000f40ee: mov r1,r5
  000f40f0: mov r2,r6
  000f40f2: movs r3,#0x0
  000f40f4: blx 0x00075db4
  000f40f8: mov r6,r0
  000f40fa: cbz r4,0x000f411a
  000f40fc: ldr r0,[0x000f4220]
  000f40fe: ldr r1,[r4,#0x0]
  000f4100: add r0,pc
  000f4102: ldr r0,[r0,#0x0]
  000f4104: ldr r0,[r0,#0x0]
  000f4106: blx 0x00071848
  000f410a: ldr r1,[r4,#0x4]
  000f410c: movs r2,#0x0
  000f410e: ldr.w r1,[r1,r0,lsl #0x2]
  000f4112: add r0,sp,#0x28
  000f4114: blx 0x0006f028
  000f4118: b 0x000f4126
  000f411a: ldr r1,[0x000f421c]
  000f411c: add r0,sp,#0x28
  000f411e: movs r2,#0x0
  000f4120: add r1,pc
  000f4122: blx 0x0006ee18
  000f4126: cbz r6,0x000f4146
  000f4128: ldr r0,[0x000f4228]
  000f412a: ldr r1,[r6,#0x0]
  000f412c: add r0,pc
  000f412e: ldr r0,[r0,#0x0]
  000f4130: ldr r0,[r0,#0x0]
  000f4132: blx 0x00071848
  000f4136: ldr r1,[r6,#0x4]
  000f4138: ldr.w r1,[r1,r0,lsl #0x2]
  000f413c: add r0,sp,#0x1c
  000f413e: movs r2,#0x0
  000f4140: blx 0x0006f028
  000f4144: b 0x000f4152
  000f4146: ldr r1,[0x000f4224]
  000f4148: add r1,pc
  000f414a: add r0,sp,#0x1c
  000f414c: movs r2,#0x0
  000f414e: blx 0x0006ee18
  000f4152: cbz r4,0x000f4164
  000f4154: mov r0,r4
  000f4156: blx 0x0006facc
  000f415a: mov r0,r4
  000f415c: blx 0x0006f64c
  000f4160: blx 0x0006eb48
  000f4164: cbz r6,0x000f4176
  000f4166: mov r0,r6
  000f4168: blx 0x0006facc
  000f416c: mov r0,r6
  000f416e: blx 0x0006f64c
  000f4172: blx 0x0006eb48
  000f4176: mov r0,r9
  000f4178: blx 0x0007213c
  000f417c: blx 0x0006eb48
  000f4180: ldr r0,[sp,#0x24]
  000f4182: cbz r0,0x000f41b2
  000f4184: ldr r1,[0x000f422c]
  000f4186: add r1,pc
  000f4188: add r0,sp,#0x4
  000f418a: movs r2,#0x0
  000f418c: blx 0x0006ee18
  000f4190: add r0,sp,#0x10
  000f4192: add r1,sp,#0x28
  000f4194: add r2,sp,#0x4
  000f4196: blx 0x0006ef98
  000f419a: add r1,sp,#0x10
  000f419c: add r2,sp,#0x1c
  000f419e: mov r0,r8
  000f41a0: blx 0x0006ef98
  000f41a4: add r0,sp,#0x10
  000f41a6: blx 0x0006ee30
  000f41aa: add r0,sp,#0x4
  000f41ac: blx 0x0006ee30
  000f41b0: b 0x000f41bc
  000f41b2: add r1,sp,#0x28
  000f41b4: mov r0,r8
  000f41b6: movs r2,#0x0
  000f41b8: blx 0x0006f028
  000f41bc: add r0,sp,#0x1c
  000f41be: blx 0x0006ee30
  000f41c2: add r0,sp,#0x28
  000f41c4: blx 0x0006ee30
  000f41c8: ldr r0,[sp,#0x34]
  000f41ca: ldr.w r1,[r10,#0x0]
  000f41ce: subs r0,r1,r0
  000f41d0: ittt eq
  000f41d2: add.eq sp,#0x38
  000f41d4: pop.eq.w {r8,r9,r10}
  000f41d8: pop.eq {r4,r5,r6,r7,pc}
  000f41da: blx 0x0006e824
```
