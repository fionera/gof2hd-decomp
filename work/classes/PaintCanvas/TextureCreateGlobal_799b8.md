# PaintCanvas::TextureCreateGlobal
elf 0x799b8 body 118
Sig: undefined __thiscall TextureCreateGlobal(PaintCanvas * this, String param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TextureCreateGlobal(AbyssEngine::String, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::TextureCreateGlobal(PaintCanvas *this,undefined4 param_2,int param_3)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  int *piVar4;
  float extraout_s0;
  
  piVar4 = *(int **)(DAT_00089a30 + 0x899cc);
  iVar1 = *piVar4;
  pcVar2 = (char *)String::GetAEChar();
  iVar3 = TextureCreateFromFile
                    (*(Engine **)(this + 0x34),pcVar2,(_func_void_Image_ptr_void_ptr *)0x0,
                     (void *)0x0,(uint *)&stack0xffffffd8,false,extraout_s0);
  if (iVar3 == 1) {
    glActiveTexture(param_3 + 0x84c0);
    glBindTexture(0xde1,0);
    glActiveTexture(0x84c0);
  }
  operator_delete__(pcVar2);
  if (*piVar4 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000899b8: push {r4,r5,r6,r7,lr}
  000899ba: add r7,sp,#0xc
  000899bc: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  000899c0: mov r6,r0
  000899c2: ldr r0,[0x00089a30]
  000899c4: movs r5,#0x0
  000899c6: mov r8,r2
  000899c8: add r0,pc
  000899ca: ldr.w r9,[r0,#0x0]
  000899ce: ldr.w r0,[r9,#0x0]
  000899d2: str r0,[sp,#0x14]
  000899d4: mov r0,r1
  000899d6: str r5,[sp,#0x10]
  000899d8: blx 0x0006ef68
  000899dc: mov r4,r0
  000899de: ldr r0,[r6,#0x34]
  000899e0: add r1,sp,#0x10
  000899e2: strd r1,r5,[sp,#0x0]
  000899e6: mov r1,r4
  000899e8: movs r2,#0x0
  000899ea: movs r3,#0x0
  000899ec: str r5,[sp,#0x8]
  000899ee: blx 0x0006fb80
  000899f2: cmp r0,#0x1
  000899f4: bne 0x00089a12
  000899f6: movw r0,#0x84c0
  000899fa: add r0,r8
  000899fc: blx 0x0006fb8c
  00089a00: ldr r1,[sp,#0x10]
  00089a02: movw r0,#0xde1
  00089a06: blx 0x0006f244
  00089a0a: movw r0,#0x84c0
  00089a0e: blx 0x0006fb8c
  00089a12: mov r0,r4
  00089a14: blx 0x0006ebfc
  00089a18: ldr r0,[sp,#0x14]
  00089a1a: ldr.w r1,[r9,#0x0]
  00089a1e: subs r0,r1,r0
  00089a20: ittt eq
  00089a22: add.eq sp,#0x18
  00089a24: pop.eq.w {r8,r9,r11}
  00089a28: pop.eq {r4,r5,r6,r7,pc}
  00089a2a: blx 0x0006e824
```
