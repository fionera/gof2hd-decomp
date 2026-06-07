# AESoundRessource::SetSound
elf 0x7fb74 body 40
Sig: undefined __thiscall SetSound(AESoundRessource * this, AESoundInfo * param_1, int param_2)

## decompile
```c

/* AbyssEngine::AESoundRessource::SetSound(AbyssEngine::AESoundInfo const*, int) */

void __thiscall
AbyssEngine::AESoundRessource::SetSound(AESoundRessource *this,AESoundInfo *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  *(int *)(this + 0x10) = param_2;
  *(AESoundInfo **)this = param_1;
  ArraySetLength<AbyssEngine::AESoundInterface*>(param_2,(Array *)(this + 4));
  iVar1 = *(int *)(this + 4);
  for (iVar2 = 0; iVar1 != iVar2; iVar2 = iVar2 + 1) {
    *(undefined4 *)(*(int *)(this + 8) + iVar2 * 4) = 0;
  }
  return;
}

```

## target disasm
```
  0008fb74: push {r4,r6,r7,lr}
  0008fb76: add r7,sp,#0x8
  0008fb78: str r2,[r0,#0x10]
  0008fb7a: mov r4,r0
  0008fb7c: str r1,[r0,#0x0]
  0008fb7e: adds r1,r0,#0x4
  0008fb80: mov r0,r2
  0008fb82: blx 0x000700b4
  0008fb86: ldr r0,[r4,#0x4]
  0008fb88: movs r1,#0x0
  0008fb8a: movs r2,#0x0
  0008fb8c: b 0x0008fb96
  0008fb8e: ldr r3,[r4,#0x8]
  0008fb90: str.w r1,[r3,r2,lsl #0x2]
  0008fb94: adds r2,#0x1
  0008fb96: cmp r0,r2
  0008fb98: bne 0x0008fb8e
  0008fb9a: pop {r4,r6,r7,pc}
```
