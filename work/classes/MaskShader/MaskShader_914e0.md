# MaskShader::MaskShader
elf 0x914e0 body 94
Sig: undefined __thiscall MaskShader(MaskShader * this)

## decompile
```c

/* AbyssEngine::MaskShader::MaskShader() */

void __thiscall AbyssEngine::MaskShader::MaskShader(MaskShader *this)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  MaskShader *apMStack_20 [3];
  int local_14;
  
  piVar4 = *(int **)(DAT_000a1558 + 0xa14ec);
  local_14 = *piVar4;
  apMStack_20[0] = this;
  ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar1 = *(undefined4 **)(DAT_000a155c + 0xa14fe);
  puVar3 = *(undefined4 **)(DAT_000a1564 + 0xa1506);
  *(int *)this = *(int *)(DAT_000a1560 + 0xa1502) + 8;
  iVar2 = DAT_000a1568;
  *puVar3 = *puVar1;
  String::String((String *)apMStack_20,(char *)(iVar2 + 0xa1516),false);
  String::operator=((String *)(this + 0xc),(String *)apMStack_20);
  String::~String((String *)apMStack_20);
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
  000a14e0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000a14e2: add r7,sp,#0x18
  000a14e4: mov r4,r0
  000a14e6: ldr r0,[0x000a1558]
  000a14e8: add r0,pc
  000a14ea: ldr r5,[r0,#0x0]
  000a14ec: ldr r0,[r5,#0x0]
  000a14ee: str r0,[sp,#0xc]
  000a14f0: mov r0,r4
  000a14f2: blx 0x00070930
  000a14f6: ldr r0,[0x000a155c]
  000a14f8: ldr r1,[0x000a1560]
  000a14fa: add r0,pc
  000a14fc: ldr r2,[0x000a1564]
  000a14fe: add r1,pc
  000a1500: ldr r0,[r0,#0x0]
  000a1502: add r2,pc
  000a1504: ldr r1,[r1,#0x0]
  000a1506: ldr r2,[r2,#0x0]
  000a1508: adds r1,#0x8
  000a150a: str r1,[r4,#0x0]
  000a150c: ldr r0,[r0,#0x0]
  000a150e: ldr r1,[0x000a1568]
  000a1510: str r0,[r2,#0x0]
  000a1512: add r1,pc
  000a1514: mov r0,sp
  000a1516: movs r2,#0x0
  000a1518: blx 0x0006ee18
  000a151c: add.w r0,r4,#0xc
  000a1520: mov r1,sp
  000a1522: blx 0x0006f2b0
  000a1526: mov r0,sp
  000a1528: blx 0x0006ee30
  000a152c: ldr r0,[sp,#0xc]
  000a152e: ldr r1,[r5,#0x0]
  000a1530: subs r0,r1,r0
  000a1532: ittt eq
  000a1534: mov.eq r0,r4
  000a1536: add.eq sp,#0x10
  000a1538: pop.eq {r4,r5,r7,pc}
  000a153a: blx 0x0006e824
```
