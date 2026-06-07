# ModStation::OnResume
elf 0xdc29c body 42
Sig: undefined __stdcall OnResume(void)

## decompile
```c

/* ModStation::OnResume() */

void ModStation::OnResume(void)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_000ec2c8 + 0xec2a6);
  if ((*piVar2 != 0) && (iVar1 = FModSound::tryToStopMusicForBGMusic(), iVar1 == 0)) {
    (*(code *)(DAT_001ac184 + 0x1ac188))(*piVar2,1,**(undefined4 **)(DAT_000ec2cc + 0xec2ba));
    return;
  }
  return;
}

```

## target disasm
```
  000ec29c: push {r4,r6,r7,lr}
  000ec29e: add r7,sp,#0x8
  000ec2a0: ldr r0,[0x000ec2c8]
  000ec2a2: add r0,pc
  000ec2a4: ldr r4,[r0,#0x0]
  000ec2a6: ldr r0,[r4,#0x0]
  000ec2a8: cbz r0,0x000ec2b0
  000ec2aa: blx 0x000756c4
  000ec2ae: cbz r0,0x000ec2b2
  000ec2b0: pop {r4,r6,r7,pc}
  000ec2b2: ldr r0,[0x000ec2cc]
  000ec2b4: movs r1,#0x1
  000ec2b6: add r0,pc
  000ec2b8: ldr r0,[r0,#0x0]
  000ec2ba: ldr r2,[r0,#0x0]
  000ec2bc: ldr r0,[r4,#0x0]
  000ec2be: pop.w {r4,r6,r7,lr}
  000ec2c2: b.w 0x001ac178
```
