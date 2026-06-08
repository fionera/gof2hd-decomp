# LensFlare::LensFlare
elf 0x11d00c body 92
Sig: undefined __thiscall LensFlare(LensFlare * this, PaintCanvas * param_1)

## decompile
```c

/* LensFlare::LensFlare(AbyssEngine::PaintCanvas*) */

LensFlare * __thiscall LensFlare::LensFlare(LensFlare *this,PaintCanvas *param_1)

{
  void *pvVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  
  pvVar1 = operator_new__(0xc);
  *(void **)(this + 0x10) = pvVar1;
  iVar5 = 0;
  puVar4 = *(undefined4 **)(DAT_0012d068 + 0x12d02c);
  for (iVar3 = 0; iVar3 != 3; iVar3 = iVar3 + 1) {
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)*puVar4,(short)iVar3 + 0x508,(uint *)(*(int *)(this + 0x10) + iVar5));
    iVar5 = iVar5 + 4;
  }
  uVar2 = AbyssEngine::PaintCanvas::GetWidth();
  *(undefined4 *)(this + 8) = uVar2;
  uVar2 = AbyssEngine::PaintCanvas::GetHeight();
  *(undefined4 *)(this + 0xc) = uVar2;
  *(PaintCanvas **)(this + 4) = param_1;
  return this;
}

```

## target disasm
```
  0012d00c: push {r4,r5,r6,r7,lr}
  0012d00e: add r7,sp,#0xc
  0012d010: push {r8,r9,r11}
  0012d014: mov r9,r0
  0012d016: movs r0,#0xc
  0012d018: mov r8,r1
  0012d01a: blx 0x0006ec08
  0012d01e: str.w r0,[r9,#0x10]
  0012d022: movs r6,#0x0
  0012d024: ldr r0,[0x0012d068]
  0012d026: movs r4,#0x0
  0012d028: add r0,pc
  0012d02a: ldr r5,[r0,#0x0]
  0012d02c: b 0x0012d044
  0012d02e: ldr.w r1,[r9,#0x10]
  0012d032: ldr r0,[r5,#0x0]
  0012d034: adds r2,r1,r6
  0012d036: add.w r1,r4,#0x508
  0012d03a: uxth r1,r1
  0012d03c: blx 0x00071cf8
  0012d040: adds r6,#0x4
  0012d042: adds r4,#0x1
  0012d044: cmp r4,#0x3
  0012d046: bne 0x0012d02e
  0012d048: mov r0,r8
  0012d04a: blx 0x0006fafc
  0012d04e: str.w r0,[r9,#0x8]
  0012d052: mov r0,r8
  0012d054: blx 0x0006fb08
  0012d058: str.w r0,[r9,#0xc]
  0012d05c: mov r0,r9
  0012d05e: str.w r8,[r9,#0x4]
  0012d062: pop.w {r8,r9,r11}
  0012d066: pop {r4,r5,r6,r7,pc}
```
