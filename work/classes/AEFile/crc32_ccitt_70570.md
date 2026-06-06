# AEFile::crc32_ccitt
elf 0x70570 body 62
Sig: undefined __thiscall crc32_ccitt(AEFile * this, String * param_1)

## decompile
```c

/* AEFile::crc32_ccitt(AbyssEngine::String const&) */

undefined8 __thiscall AEFile::crc32_ccitt(AEFile *this,String *param_1)

{
  String *pSVar1;
  ushort *puVar2;
  uint uVar3;
  int iVar4;
  
  uVar3 = 0;
  pSVar1 = (String *)(DAT_000805b0 + 0x80584);
  for (iVar4 = 0; iVar4 < *(int *)(this + 8); iVar4 = iVar4 + 1) {
    puVar2 = (ushort *)AbyssEngine::String::operator[]((String *)this,iVar4);
    uVar3 = *(uint *)(pSVar1 + ((*puVar2 ^ uVar3) & 0xff) * 4) ^ uVar3 >> 8;
    param_1 = pSVar1;
  }
  return CONCAT44(param_1,uVar3);
}

```
## target disasm
```
  00080570: push {r4,r5,r6,r7,lr}
  00080572: add r7,sp,#0xc
  00080574: push.w r8
  00080578: mov r5,r0
  0008057a: ldr r0,[0x000805b0]
  0008057c: movs r4,#0x0
  0008057e: movs r6,#0x0
  00080580: add r0,pc
  00080582: mov r8,r0
  00080584: b 0x000805a0
  00080586: mov r0,r5
  00080588: mov r1,r6
  0008058a: blx 0x0006f37c
  0008058e: ldrh r0,[r0,#0x0]
  00080590: mov r1,r8
  00080592: adds r6,#0x1
  00080594: eors r0,r4
  00080596: uxtb r0,r0
  00080598: ldr.w r0,[r8,r0,lsl #0x2]
  0008059c: eor.w r4,r0,r4, lsr #0x8
  000805a0: ldr r0,[r5,#0x8]
  000805a2: cmp r6,r0
  000805a4: blt 0x00080586
  000805a6: mov r0,r4
  000805a8: pop.w r8
  000805ac: pop {r4,r5,r6,r7,pc}
```
