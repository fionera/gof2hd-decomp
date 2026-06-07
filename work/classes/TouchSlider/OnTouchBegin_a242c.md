# TouchSlider::OnTouchBegin
elf 0xa242c body 28
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* TouchSlider::OnTouchBegin(int, int) */

undefined4 TouchSlider::OnTouchBegin(int param_1,int param_2)

{
  undefined4 uVar1;
  int in_r2;
  
  if (*(char *)(param_1 + 0x35) != '\0') {
    return 0;
  }
  uVar1 = touchedInside((TouchSlider *)param_1,param_2,in_r2);
  *(char *)(param_1 + 0x34) = (char)uVar1;
  return uVar1;
}

```

## target disasm
```
  000b242c: push {r4,r6,r7,lr}
  000b242e: add r7,sp,#0x8
  000b2430: mov r4,r0
  000b2432: ldrb.w r0,[r0,#0x35]
  000b2436: cbz r0,0x000b243c
  000b2438: movs r0,#0x0
  000b243a: pop {r4,r6,r7,pc}
  000b243c: mov r0,r4
  000b243e: blx 0x00072dcc
  000b2442: strb.w r0,[r4,#0x34]
  000b2446: pop {r4,r6,r7,pc}
```
