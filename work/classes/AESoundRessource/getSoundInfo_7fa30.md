# AESoundRessource::getSoundInfo
elf 0x7fa30 body 72
Sig: undefined __thiscall getSoundInfo(AESoundRessource * this, int param_1, AESoundInfo * param_2, int * param_3)

## decompile
```c

/* AbyssEngine::AESoundRessource::getSoundInfo(int, AbyssEngine::AESoundInfo&, int&) */

void __thiscall
AbyssEngine::AESoundRessource::getSoundInfo
          (AESoundRessource *this,int param_1,AESoundInfo *param_2,int *param_3)

{
  undefined8 *puVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  
  iVar3 = 0;
  *param_3 = -1;
  uVar2 = 0;
  while( true ) {
    if (*(uint *)(this + 4) <= uVar2) {
      return;
    }
    if (*(int *)(*(int *)this + iVar3) == param_1) break;
    iVar3 = iVar3 + 0xc;
    uVar2 = uVar2 + 1;
  }
  puVar1 = (undefined8 *)(*(int *)this + iVar3);
  *param_3 = uVar2;
  uVar4 = *puVar1;
  *(undefined4 *)(param_2 + 8) = *(undefined4 *)(puVar1 + 1);
  *(undefined8 *)param_2 = uVar4;
  return;
}

```

## target disasm
```
  0008fa30: push {r4,r5,r6,r7,lr}
  0008fa32: add r7,sp,#0xc
  0008fa34: push.w r11
  0008fa38: mov.w r5,#0xffffffff
  0008fa3c: mov.w r12,#0x0
  0008fa40: str r5,[r3,#0x0]
  0008fa42: movs r4,#0x0
  0008fa44: ldr.w lr,[r0,#0x4]
  0008fa48: b 0x0008fa5a
  0008fa4a: ldr r5,[r0,#0x0]
  0008fa4c: ldr.w r6,[r5,r12]
  0008fa50: cmp r6,r1
  0008fa52: beq 0x0008fa60
  0008fa54: add.w r12,r12,#0xc
  0008fa58: adds r4,#0x1
  0008fa5a: cmp r4,lr
  0008fa5c: bcc 0x0008fa4a
  0008fa5e: b 0x0008fa72
  0008fa60: add.w r0,r5,r12
  0008fa64: str r4,[r3,#0x0]
  0008fa66: vldr.64 d16,[r0]
  0008fa6a: ldr r0,[r0,#0x8]
  0008fa6c: str r0,[r2,#0x8]
  0008fa6e: vstr.64 d16,[r2]
  0008fa72: pop.w r11
  0008fa76: pop {r4,r5,r6,r7,pc}
```
