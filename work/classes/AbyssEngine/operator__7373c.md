# AbyssEngine::operator+
elf 0x7373c body 90
Sig: undefined __thiscall operator+(AbyssEngine * this, String * param_1, int * param_2)

## decompile
```c

/* AbyssEngine::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::String const&, int const&) */

void __thiscall AbyssEngine::operator+(AbyssEngine *this,String *param_1,int *param_2)

{
  int *piVar1;
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_000837b0 + 0x83752);
  local_1c = *piVar1;
  String::String(aSStack_28,param_1,false);
  String::String(aSStack_34,*param_2);
  String::operator+=(aSStack_28,aSStack_34);
  String::~String(aSStack_34);
  String::String((String *)this,aSStack_28,false);
  String::~String(aSStack_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008373c: push {r4,r5,r6,r7,lr}
  0008373e: add r7,sp,#0xc
  00083740: push.w r11
  00083744: sub sp,#0x20
  00083746: mov r4,r0
  00083748: ldr r0,[0x000837b0]
  0008374a: mov r5,r2
  0008374c: movs r2,#0x0
  0008374e: add r0,pc
  00083750: ldr r6,[r0,#0x0]
  00083752: ldr r0,[r6,#0x0]
  00083754: str r0,[sp,#0x1c]
  00083756: add r0,sp,#0x10
  00083758: blx 0x0006f5b0
  0008375c: ldr r1,[r5,#0x0]
  0008375e: add r0,sp,#0x4
  00083760: blx 0x0006f658
  00083764: add r0,sp,#0x10
  00083766: add r1,sp,#0x4
  00083768: blx 0x0006ef5c
  0008376c: add r0,sp,#0x4
  0008376e: blx 0x0006f580
  00083772: add r1,sp,#0x10
  00083774: mov r0,r4
  00083776: movs r2,#0x0
  00083778: blx 0x0006f5b0
  0008377c: add r0,sp,#0x10
  0008377e: blx 0x0006f580
  00083782: ldr r0,[sp,#0x1c]
  00083784: ldr r1,[r6,#0x0]
  00083786: subs r0,r1,r0
  00083788: ittt eq
  0008378a: add.eq sp,#0x20
  0008378c: pop.eq.w r11
  00083790: pop.eq {r4,r5,r6,r7,pc}
  00083792: blx 0x0006e824
```
