# FModSound::updateEvent3DAttributes
elf 0x93588 body 32
Sig: undefined __thiscall updateEvent3DAttributes(FModSound * this, int param_1, Vector * param_2, Vector * param_3, bool param_4)

## decompile
```c

/* FModSound::updateEvent3DAttributes(int, AbyssEngine::AEMath::Vector*,
   AbyssEngine::AEMath::Vector*, bool) */

void __thiscall
FModSound::updateEvent3DAttributes
          (FModSound *this,int param_1,Vector *param_2,Vector *param_3,bool param_4)

{
  undefined4 uVar1;
  
  uVar1 = updateEvent3DAttributes
                    (this,*(Event **)(this + param_1 * 4 + 0x18),param_1,param_2,param_3,param_4);
  *(undefined4 *)(this + param_1 * 4 + 0x18) = uVar1;
  return;
}

```

## target disasm
```
  000a3588: push {r2,r3,r4,r6,r7,lr}
  000a358a: add r7,sp,#0x10
  000a358c: add.w r4,r0,r1, lsl #0x2
  000a3590: ldr.w lr,[r7,#0x8]
  000a3594: mov r12,r2
  000a3596: mov r2,r1
  000a3598: ldr r1,[r4,#0x18]
  000a359a: strd r3,lr,[sp,#0x0]
  000a359e: mov r3,r12
  000a35a0: blx 0x000715b4
  000a35a4: str r0,[r4,#0x18]
  000a35a6: pop {r2,r3,r4,r6,r7,pc}
```
