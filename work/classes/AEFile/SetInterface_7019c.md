# AEFile::SetInterface
elf 0x7019c body 96
Sig: undefined __stdcall SetInterface(FileInterface * param_1)

## decompile
```c

/* AEFile::SetInterface(FileInterface*) */

void AEFile::SetInterface(FileInterface *param_1)

{
  Array<AEPakFileEntry*> *this;
  Array<AELowLevelFile*> *this_00;
  int *piVar1;
  
  if ((param_1 != (FileInterface *)0x0) && (param_1[4] != (FileInterface)0x0)) {
    if (**(int **)(DAT_0008020c + 0x801b2) != 0) {
      (**(code **)(*(int *)param_1 + 0x34))(param_1);
    }
    piVar1 = *(int **)(DAT_00080210 + 0x801c4);
    if (*piVar1 == 0) {
      this = operator_new(0xc);
      Array<AEPakFileEntry*>::Array(this);
      *piVar1 = (int)this;
    }
    piVar1 = *(int **)(DAT_00080214 + 0x801dc);
    if (*piVar1 == 0) {
      this_00 = operator_new(0xc);
      Array<AELowLevelFile*>::Array(this_00);
      *piVar1 = (int)this_00;
    }
    **(undefined4 **)(DAT_00080218 + 0x801f4) = param_1;
  }
  return;
}

```
## target disasm
```
  0008019c: push {r4,r5,r6,r7,lr}
  0008019e: add r7,sp,#0xc
  000801a0: push.w r11
  000801a4: mov r4,r0
  000801a6: cbz r0,0x000801f6
  000801a8: ldrb r0,[r4,#0x4]
  000801aa: cbz r0,0x000801f6
  000801ac: ldr r0,[0x0008020c]
  000801ae: add r0,pc
  000801b0: ldr r0,[r0,#0x0]
  000801b2: ldr r1,[r0,#0x0]
  000801b4: cbz r1,0x000801be
  000801b6: ldr r0,[r4,#0x0]
  000801b8: ldr r2,[r0,#0x34]
  000801ba: mov r0,r4
  000801bc: blx r2
  000801be: ldr r0,[0x00080210]
  000801c0: add r0,pc
  000801c2: ldr r6,[r0,#0x0]
  000801c4: ldr r0,[r6,#0x0]
  000801c6: cbnz r0,0x000801d6
  000801c8: movs r0,#0xc
  000801ca: blx 0x0006eb24
  000801ce: mov r5,r0
  000801d0: blx 0x0006f310
  000801d4: str r5,[r6,#0x0]
  000801d6: ldr r0,[0x00080214]
  000801d8: add r0,pc
  000801da: ldr r6,[r0,#0x0]
  000801dc: ldr r0,[r6,#0x0]
  000801de: cbnz r0,0x000801ee
  000801e0: movs r0,#0xc
  000801e2: blx 0x0006eb24
  000801e6: mov r5,r0
  000801e8: blx 0x0006f31c
  000801ec: str r5,[r6,#0x0]
  000801ee: ldr r0,[0x00080218]
  000801f0: add r0,pc
  000801f2: ldr r0,[r0,#0x0]
  000801f4: str r4,[r0,#0x0]
  000801f6: pop.w r11
  000801fa: pop {r4,r5,r6,r7,pc}
```
