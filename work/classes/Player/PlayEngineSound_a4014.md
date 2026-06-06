# Player::PlayEngineSound
elf 0xa4014 body 102
Sig: undefined __stdcall PlayEngineSound(int param_1, Vector * param_2)

## decompile
```c

/* Player::PlayEngineSound(int, AbyssEngine::AEMath::Vector*) */

void Player::PlayEngineSound(int param_1,Vector *param_2)

{
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  Matrix aMStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_000b407c + 0xb4028);
  iVar1 = *(int *)(DAT_000b4080 + 0xb402a);
  local_1c = *piVar3;
  *(Vector **)(param_1 + 0xf4) = param_2;
  if (*(char *)(iVar1 + 0xf) != '\0') {
    AbyssEngine::AEMath::MatrixGetPosition(aMStack_28);
    uVar2 = FModSound::updateEvent3DAttributes
                      ((FModSound *)**(undefined4 **)(DAT_000b4084 + 0xb4052),
                       *(Event **)(param_1 + 0xf0),*(int *)(param_1 + 0xf4),(Vector *)aMStack_28,
                       (Vector *)0x0,true);
    *(undefined4 *)(param_1 + 0xf0) = uVar2;
    *(undefined1 *)(param_1 + 0x108) = 1;
  }
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000b4014: push {r4,r5,r6,r7,lr}
  000b4016: add r7,sp,#0xc
  000b4018: push.w r8
  000b401c: sub sp,#0x18
  000b401e: mov r4,r0
  000b4020: ldr r0,[0x000b407c]
  000b4022: ldr r2,[0x000b4080]
  000b4024: add r0,pc
  000b4026: add r2,pc
  000b4028: ldr r6,[r0,#0x0]
  000b402a: ldr r0,[r2,#0x0]
  000b402c: ldr r2,[r6,#0x0]
  000b402e: str r2,[sp,#0x14]
  000b4030: str.w r1,[r4,#0xf4]
  000b4034: ldrb r0,[r0,#0xf]
  000b4036: cbz r0,0x000b4066
  000b4038: add.w r8,sp,#0x8
  000b403c: adds r1,r4,#0x4
  000b403e: mov r0,r8
  000b4040: blx 0x0006f16c
  000b4044: ldr r0,[0x000b4084]
  000b4046: movs r5,#0x1
  000b4048: ldrd r1,r2,[r4,#0xf0]
  000b404c: movs r3,#0x0
  000b404e: add r0,pc
  000b4050: ldr r0,[r0,#0x0]
  000b4052: ldr r0,[r0,#0x0]
  000b4054: strd r3,r5,[sp,#0x0]
  000b4058: mov r3,r8
  000b405a: blx 0x000715b4
  000b405e: str.w r0,[r4,#0xf0]
  000b4062: strb.w r5,[r4,#0x108]
  000b4066: ldr r0,[sp,#0x14]
  000b4068: ldr r1,[r6,#0x0]
  000b406a: subs r0,r1,r0
  000b406c: ittt eq
  000b406e: add.eq sp,#0x18
  000b4070: pop.eq.w r8
  000b4074: pop.eq {r4,r5,r6,r7,pc}
  000b4076: blx 0x0006e824
```
