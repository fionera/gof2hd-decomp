# ApplicationManager::ConvertTouchCoords
elf 0x839de body 90
Sig: undefined __thiscall ConvertTouchCoords(ApplicationManager * this, int * param_1, int * param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::ConvertTouchCoords(int&, int&) */

void __thiscall
AbyssEngine::ApplicationManager::ConvertTouchCoords
          (ApplicationManager *this,int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(*(int *)this + 0x30);
  if (iVar2 != 3) {
    if (iVar2 != 1) {
      if (iVar2 != 0) {
        return;
      }
      iVar2 = *param_1;
      iVar1 = PaintCanvas::GetWidth();
      *param_1 = iVar1 - *param_2;
      goto LAB_00093a30;
    }
    iVar1 = *param_1;
    *param_1 = *param_2;
    iVar2 = PaintCanvas::GetHeight();
    *param_2 = iVar2 - iVar1;
  }
  iVar2 = PaintCanvas::GetWidth();
  *param_1 = iVar2 - *param_1;
  iVar2 = PaintCanvas::GetHeight();
  iVar2 = iVar2 - *param_2;
LAB_00093a30:
  *param_2 = iVar2;
  return;
}

```

## target disasm
```
  000939de: push {r4,r5,r6,r7,lr}
  000939e0: add r7,sp,#0xc
  000939e2: push.w r8
  000939e6: mov r6,r0
  000939e8: ldr r0,[r0,#0x0]
  000939ea: mov r5,r1
  000939ec: mov r4,r2
  000939ee: ldr r1,[r0,#0x30]
  000939f0: cmp r1,#0x3
  000939f2: beq 0x00093a1c
  000939f4: cmp r1,#0x1
  000939f6: beq 0x00093a08
  000939f8: cbnz r1,0x00093a32
  000939fa: ldr r6,[r5,#0x0]
  000939fc: blx 0x0006fafc
  00093a00: ldr r1,[r4,#0x0]
  00093a02: subs r0,r0,r1
  00093a04: str r0,[r5,#0x0]
  00093a06: b 0x00093a30
  00093a08: ldr r1,[r4,#0x0]
  00093a0a: ldr.w r8,[r5,#0x0]
  00093a0e: str r1,[r5,#0x0]
  00093a10: blx 0x0006fb08
  00093a14: sub.w r0,r0,r8
  00093a18: str r0,[r4,#0x0]
  00093a1a: ldr r0,[r6,#0x0]
  00093a1c: blx 0x0006fafc
  00093a20: ldr r1,[r5,#0x0]
  00093a22: subs r0,r0,r1
  00093a24: str r0,[r5,#0x0]
  00093a26: ldr r0,[r6,#0x0]
  00093a28: blx 0x0006fb08
  00093a2c: ldr r1,[r4,#0x0]
  00093a2e: subs r6,r0,r1
  00093a30: str r6,[r4,#0x0]
  00093a32: pop.w r8
  00093a36: pop {r4,r5,r6,r7,pc}
```
