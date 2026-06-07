# MGame::MGame
elf 0x177b30 body 206
Sig: undefined __thiscall MGame(MGame * this)

## decompile
```c

/* MGame::MGame() */

MGame * __thiscall MGame::MGame(MGame *this)

{
  SingleLevel *pSVar1;
  DialogueWindow *pDVar2;
  int *piVar3;
  int iVar4;
  
  this->field_0 = (int *)(*(int *)(DAT_00187c04 + 0x187b3c) + 8);
  AbyssEngine::String::String((String *)&this->field_64);
  iVar4 = DAT_00187c00;
  pSVar1 = *(SingleLevel **)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  pDVar2 = *(DialogueWindow **)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  piVar3 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  this->field_A4 = 0;
  this->field_A8 = (int)pSVar1;
  this->field_AC = (int)pDVar2;
  this->field_B0 = (int)piVar3;
  this[1].m_pLevel = (SingleLevel *)0x0;
  this[1].field_78 = (int *)pSVar1;
  this[1].field_7C = (int *)pDVar2;
  this[1].field_80 = piVar3;
  *(undefined4 *)&this[1].field_C4 = 0;
  this[2].field_0 = (int *)pSVar1;
  this[2].field_4 = (int *)pDVar2;
  this[2].field_8 = piVar3;
  this->field_48 = 0;
  this->field_4C = 0;
  this[1].field_1C = 0;
  this[1].field_20 = 0;
  this[1].field_24 = 0;
  this[2].field_2C = 0;
  this[2].field_30 = 0;
  this[2].field_34 = 0;
  this->field_B4 = 0;
  this->field_B8 = 0;
  this[1].field_84 = (int *)0x0;
  this[1].m_pDialogueWindow = (DialogueWindow *)0x0;
  this[2].field_0C = 0;
  this[2].field_10 = 0;
  this->field_0C = 100;
  this->field_10 = -1;
  this->field_14 = 0;
  this->field_30 = 0;
  this->field_34 = (int)pSVar1;
  this->field_38 = (int)pDVar2;
  this->field_3C = (int)piVar3;
  this->field_20 = 0;
  this->field_24 = (int)pSVar1;
  this->field_28 = (int)pDVar2;
  this->field_2C = (int)piVar3;
  this->field_80 = (int *)0x0;
  this->field_84 = (int *)pSVar1;
  this->m_pDialogueWindow = pDVar2;
  this->field_8C = (int)piVar3;
  this->field_70 = (int *)0x0;
  this->m_pLevel = pSVar1;
  this->field_78 = (int *)pDVar2;
  this->field_7C = piVar3;
  *(undefined1 *)((int)&this[2].field_14 + 2) = 0;
  *(undefined1 *)&this->field_54 = 0;
  *(undefined2 *)((int)&this[1].field_0C + 1) = 0;
  this[1].field_4C = 0;
  this[1].field_2C = 0;
  *(undefined1 *)&this[2].field_54 = 0;
  this[2].field_44 = 0;
  this->field_40 = 0;
  this->m_pPlayerEgo = (PlayerEgo *)0x0;
  this->field_5C = 0;
  *(undefined2 *)&this->field_60 = 0;
  this->field_90 = (int *)0x0;
  this->field_94 = 0;
  *(undefined1 *)&this[1].field_0 = 0;
  *(undefined4 *)&this->field_C4 = 0;
  *(undefined2 *)((int)&this[1].field_4 + 2) = 0;
  *(undefined4 *)((int)&this[1].field_0 + 2) = 0;
  this[1].field_10 = 0;
  this[1].field_14 = 0;
  this[2].field_48 = iVar4;
  this[2].field_50 = 0;
  return this;
}

```

## target disasm
```
  00187b30: push {r4,r6,r7,lr}
  00187b32: add r7,sp,#0x8
  00187b34: mov r4,r0
  00187b36: ldr r0,[0x00187c04]
  00187b38: add r0,pc
  00187b3a: ldr r0,[r0,#0x0]
  00187b3c: adds r0,#0x8
  00187b3e: str r0,[r4,#0x0]
  00187b40: add.w r0,r4,#0x64
  00187b44: blx 0x0006efbc
  00187b48: vmov.i32 q8,#0x0
  00187b4c: add.w r1,r4,#0xa4
  00187b50: movs r2,#0x64
  00187b52: mov.w r3,#0xffffffff
  00187b56: ldr r0,[0x00187c00]
  00187b58: vst1.32 {d16,d17},[r1]
  00187b5c: add.w r1,r4,#0x13c
  00187b60: vst1.32 {d16,d17},[r1]
  00187b64: add.w r1,r4,#0x18c
  00187b68: vst1.32 {d16,d17},[r1]
  00187b6c: movs r1,#0x0
  00187b6e: strd r1,r1,[r4,#0x48]
  00187b72: strd r1,r1,[r4,#0xe4]
  00187b76: str.w r1,[r4,#0xec]
  00187b7a: strd r1,r1,[r4,#0x1bc]
  00187b7e: str.w r1,[r4,#0x1c4]
  00187b82: strd r1,r1,[r4,#0xb4]
  00187b86: strd r1,r1,[r4,#0x14c]
  00187b8a: strd r1,r1,[r4,#0x19c]
  00187b8e: strd r2,r3,[r4,#0xc]
  00187b92: add.w r2,r4,#0x30
  00187b96: str r1,[r4,#0x14]
  00187b98: vst1.64 {d16,d17},[r2]
  00187b9c: add.w r2,r4,#0x20
  00187ba0: vst1.64 {d16,d17},[r2]
  00187ba4: add.w r2,r4,#0x80
  00187ba8: vst1.64 {d16,d17},[r2]
  00187bac: add.w r2,r4,#0x70
  00187bb0: vst1.64 {d16,d17},[r2]
  00187bb4: strb.w r1,[r4,#0x1a6]
  00187bb8: strb.w r1,[r4,#0x54]
  00187bbc: strh.w r1,[r4,#0xd5]
  00187bc0: str.w r1,[r4,#0x114]
  00187bc4: str.w r1,[r4,#0xf4]
  00187bc8: strb.w r1,[r4,#0x1e4]
  00187bcc: str.w r1,[r4,#0x1d4]
  00187bd0: str r1,[r4,#0x40]
  00187bd2: strd r1,r1,[r4,#0x58]
  00187bd6: strh.w r1,[r4,#0x60]
  00187bda: strd r1,r1,[r4,#0x90]
  00187bde: strb.w r1,[r4,#0xc8]
  00187be2: str.w r1,[r4,#0xc4]
  00187be6: strh.w r1,[r4,#0xce]
  00187bea: str.w r1,[r4,#0xca]
  00187bee: strd r1,r1,[r4,#0xd8]
  00187bf2: str.w r0,[r4,#0x1d8]
  00187bf6: mov r0,r4
  00187bf8: str.w r1,[r4,#0x1e0]
  00187bfc: pop {r4,r6,r7,pc}
```
