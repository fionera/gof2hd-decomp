# ModMainMenu::OnResume
elf 0x1757f8 body 42
Sig: undefined __stdcall OnResume(void)

## decompile
```c

/* ModMainMenu::OnResume() */

void ModMainMenu::OnResume(void)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_00185824 + 0x185802);
  if ((*piVar2 != 0) && (iVar1 = FModSound::tryToStopMusicForBGMusic(), iVar1 == 0)) {
    (*(code *)(DAT_001ac184 + 0x1ac188))(*piVar2,1,**(undefined4 **)(DAT_00185828 + 0x185816));
    return;
  }
  return;
}

```

## target disasm
```
  001857f8: push {r4,r6,r7,lr}
  001857fa: add r7,sp,#0x8
  001857fc: ldr r0,[0x00185824]
  001857fe: add r0,pc
  00185800: ldr r4,[r0,#0x0]
  00185802: ldr r0,[r4,#0x0]
  00185804: cbz r0,0x0018580c
  00185806: blx 0x000756c4
  0018580a: cbz r0,0x0018580e
  0018580c: pop {r4,r6,r7,pc}
  0018580e: ldr r0,[0x00185828]
  00185810: movs r1,#0x1
  00185812: add r0,pc
  00185814: ldr r0,[r0,#0x0]
  00185816: ldr r2,[r0,#0x0]
  00185818: ldr r0,[r4,#0x0]
  0018581a: pop.w {r4,r6,r7,lr}
  0018581e: b.w 0x001ac178
```
