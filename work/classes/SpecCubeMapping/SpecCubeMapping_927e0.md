# SpecCubeMapping::SpecCubeMapping
elf 0x927e0 body 94
Sig: undefined __thiscall SpecCubeMapping(SpecCubeMapping * this)

## decompile
```c

/* AbyssEngine::SpecCubeMapping::SpecCubeMapping() */

void __thiscall AbyssEngine::SpecCubeMapping::SpecCubeMapping(SpecCubeMapping *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  SpecCubeMapping *apSStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a2858 + 0xa27ec);
  local_14 = *piVar4;
  apSStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a285c + 0xa27fe);
  puVar3 = *(undefined4 **)(DAT_000a2864 + 0xa2806);
  *(int *)this = *(int *)(DAT_000a2860 + 0xa2802) + 8;
  iVar2 = DAT_000a2868;
  *puVar3 = *puVar1;
  String::String((String *)apSStack_20,(char *)(iVar2 + 0xa2816),false);
  String::operator=((String *)(this + 0xc),(String *)apSStack_20);
  String::~String((String *)apSStack_20);
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
  000a27e0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a27e2: add r7,sp,#0x18
  000a27e4: mov r4,r0
  000a27e6: ldr r0,[0x000a2858]
  000a27e8: add r0,pc
  000a27ea: ldr r5,[r0,#0x0]
  000a27ec: ldr r0,[r5,#0x0]
  000a27ee: str r0,[sp,#0xc]
  000a27f0: mov r0,r4
  000a27f2: blx 0x00070930
  000a27f6: ldr r0,[0x000a285c]
  000a27f8: ldr r1,[0x000a2860]
  000a27fa: add r0,pc
  000a27fc: ldr r2,[0x000a2864]
  000a27fe: add r1,pc
  000a2800: ldr r0,[r0,#0x0]
  000a2802: add r2,pc
  000a2804: ldr r1,[r1,#0x0]
  000a2806: ldr r2,[r2,#0x0]
  000a2808: adds r1,#0x8
  000a280a: str r1,[r4,#0x0]
  000a280c: ldr r0,[r0,#0x0]
  000a280e: ldr r1,[0x000a2868]
  000a2810: str r0,[r2,#0x0]
  000a2812: add r1,pc
  000a2814: mov r0,sp
  000a2816: movs r2,#0x0
  000a2818: blx 0x0006ee18
  000a281c: add.w r0,r4,#0xc
  000a2820: mov r1,sp
  000a2822: blx 0x0006f2b0
  000a2826: mov r0,sp
  000a2828: blx 0x0006ee30
  000a282c: ldr r0,[sp,#0xc]
  000a282e: ldr r1,[r5,#0x0]
  000a2830: subs r0,r1,r0
  000a2832: ittt eq
  000a2834: mov.eq r0,r4
  000a2836: add.eq sp,#0x10
  000a2838: pop.eq {r4,r5,r7,pc}
  000a283a: blx 0x0006e824
```
