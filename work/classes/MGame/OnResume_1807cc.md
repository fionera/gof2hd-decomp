# MGame::OnResume
elf 0x1807cc body 52
Sig: undefined __stdcall OnResume(void)

## decompile
```c

/* MGame::OnResume() */

void MGame::OnResume(void)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_001907f8 + 0x1907d6);
  if ((*piVar2 != 0) && (iVar1 = FModSound::tryToStopMusicForBGMusic(), iVar1 == 0)) {
    (*(code *)(DAT_001ac184 + 0x1ac188))(*piVar2,1,**(undefined4 **)(DAT_001907fc + 0x1907ea));
    return;
  }
  return;
}

```

## target disasm
```
  001907cc: push {r4,r6,r7,lr}
  001907ce: add r7,sp,#0x8
  001907d0: ldr r0,[0x001907f8]
  001907d2: add r0,pc
  001907d4: ldr r4,[r0,#0x0]
  001907d6: ldr r0,[r4,#0x0]
  001907d8: cbz r0,0x001907e0
  001907da: blx 0x000756c4
  001907de: cbz r0,0x001907e2
  001907e0: pop {r4,r6,r7,pc}
  001907e2: ldr r0,[0x001907fc]
  001907e4: movs r1,#0x1
  001907e6: add r0,pc
  001907e8: ldr r0,[r0,#0x0]
  001907ea: ldr r2,[r0,#0x0]
  001907ec: ldr r0,[r4,#0x0]
  001907ee: pop.w {r4,r6,r7,lr}
  001907f2: b.w 0x001ac178
  001ac178: bx pc
  001ac17c: ldr r12,[0x1ac184]
  001ac180: add pc,r12,pc
```
