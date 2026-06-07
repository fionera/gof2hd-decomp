# ColorMixAdd::ColorMixAdd
elf 0x9213c body 94
Sig: undefined __thiscall ColorMixAdd(ColorMixAdd * this)

## decompile
```c

/* AbyssEngine::ColorMixAdd::ColorMixAdd() */

void __thiscall AbyssEngine::ColorMixAdd::ColorMixAdd(ColorMixAdd *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  ColorMixAdd *apCStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a21b4 + 0xa2148);
  local_14 = *piVar4;
  apCStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a21b8 + 0xa215a);
  puVar3 = *(undefined4 **)(DAT_000a21c0 + 0xa2162);
  *(int *)this = *(int *)(DAT_000a21bc + 0xa215e) + 8;
  iVar2 = DAT_000a21c4;
  *puVar3 = *puVar1;
  String::String((String *)apCStack_20,(char *)(iVar2 + 0xa2172),false);
  String::operator=((String *)(this + 0xc),(String *)apCStack_20);
  String::~String((String *)apCStack_20);
  iVar2 = *piVar4 - local_14;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  000a213c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a213e: add r7,sp,#0x18
  000a2140: mov r4,r0
  000a2142: ldr r0,[0x000a21b4]
  000a2144: add r0,pc
  000a2146: ldr r5,[r0,#0x0]
  000a2148: ldr r0,[r5,#0x0]
  000a214a: str r0,[sp,#0xc]
  000a214c: mov r0,r4
  000a214e: blx 0x00070930
  000a2152: ldr r0,[0x000a21b8]
  000a2154: ldr r1,[0x000a21bc]
  000a2156: add r0,pc
  000a2158: ldr r2,[0x000a21c0]
  000a215a: add r1,pc
  000a215c: ldr r0,[r0,#0x0]
  000a215e: add r2,pc
  000a2160: ldr r1,[r1,#0x0]
  000a2162: ldr r2,[r2,#0x0]
  000a2164: adds r1,#0x8
  000a2166: str r1,[r4,#0x0]
  000a2168: ldr r0,[r0,#0x0]
  000a216a: ldr r1,[0x000a21c4]
  000a216c: str r0,[r2,#0x0]
  000a216e: add r1,pc
  000a2170: mov r0,sp
  000a2172: movs r2,#0x0
  000a2174: blx 0x0006ee18
  000a2178: add.w r0,r4,#0xc
  000a217c: mov r1,sp
  000a217e: blx 0x0006f2b0
  000a2182: mov r0,sp
  000a2184: blx 0x0006ee30
  000a2188: ldr r0,[sp,#0xc]
  000a218a: ldr r1,[r5,#0x0]
  000a218c: subs r0,r1,r0
  000a218e: ittt eq
  000a2190: mov.eq r0,r4
  000a2192: add.eq sp,#0x10
  000a2194: pop.eq {r4,r5,r7,pc}
  000a2196: blx 0x0006e824
```
