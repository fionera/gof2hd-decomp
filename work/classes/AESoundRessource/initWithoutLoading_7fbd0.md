# AESoundRessource::initWithoutLoading
elf 0x7fbd0 body 92
Sig: undefined __thiscall initWithoutLoading(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::initWithoutLoading(int) */

void __thiscall
AbyssEngine::AESoundRessource::initWithoutLoading(AESoundRessource *this,int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  AESoundInfo aAStack_2c [4];
  int local_28;
  int local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0008fc2c + 0x8fbe4);
  local_1c = *piVar3;
  local_28 = DAT_0008fc30 + 0x8fbf0;
  getSoundInfo(this,param_1,aAStack_2c,&local_20);
  if ((local_20 != -1) && (iVar2 = *(int *)(this + 8), *(int *)(iVar2 + local_20 * 4) == 0)) {
    piVar1 = operator_new(4);
    *piVar1 = *(int *)(DAT_0008fc34 + 0x8fc10) + 8;
    *(int **)(iVar2 + local_20 * 4) = piVar1;
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
  0008fbd0: push {r4,r5,r6,r7,lr}
  0008fbd2: add r7,sp,#0xc
  0008fbd4: push.w r11
  0008fbd8: sub sp,#0x18
  0008fbda: mov r4,r0
  0008fbdc: ldr r0,[0x0008fc2c]
  0008fbde: add r3,sp,#0x10
  0008fbe0: add r0,pc
  0008fbe2: ldr r5,[r0,#0x0]
  0008fbe4: ldr r0,[r5,#0x0]
  0008fbe6: ldr r2,[0x0008fc30]
  0008fbe8: str r0,[sp,#0x14]
  0008fbea: mov r0,r4
  0008fbec: add r2,pc
  0008fbee: str r2,[sp,#0x8]
  0008fbf0: add r2,sp,#0x4
  0008fbf2: blx 0x000700c0
  0008fbf6: ldr r6,[sp,#0x10]
  0008fbf8: adds r0,r6,#0x1
  0008fbfa: beq 0x0008fc18
  0008fbfc: ldr r4,[r4,#0x8]
  0008fbfe: ldr.w r0,[r4,r6,lsl #0x2]
  0008fc02: cbnz r0,0x0008fc18
  0008fc04: movs r0,#0x4
  0008fc06: blx 0x0006eb24
  0008fc0a: ldr r1,[0x0008fc34]
  0008fc0c: add r1,pc
  0008fc0e: ldr r1,[r1,#0x0]
  0008fc10: adds r1,#0x8
  0008fc12: str r1,[r0,#0x0]
  0008fc14: str.w r0,[r4,r6,lsl #0x2]
  0008fc18: ldr r0,[sp,#0x14]
  0008fc1a: ldr r1,[r5,#0x0]
  0008fc1c: subs r0,r1,r0
  0008fc1e: ittt eq
  0008fc20: add.eq sp,#0x18
  0008fc22: pop.eq.w r11
  0008fc26: pop.eq {r4,r5,r6,r7,pc}
  0008fc28: blx 0x0006e824
```
