# PaintCanvas::RemoveAllMatsForGlow
elf 0x7d484 body 52
Sig: undefined __thiscall RemoveAllMatsForGlow(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::RemoveAllMatsForGlow() */

void __thiscall AbyssEngine::PaintCanvas::RemoveAllMatsForGlow(PaintCanvas *this)

{
  code *UNRECOVERED_JUMPTABLE;
  
  ArrayRemoveAll<AbyssEngine::Mesh*>((Array *)(this + 0x18c));
  UNRECOVERED_JUMPTABLE = *(code **)(DAT_0008d4b8 + 0x8d498);
  (*UNRECOVERED_JUMPTABLE)(this + 0x198);
  (*UNRECOVERED_JUMPTABLE)(this + 0x1a4);
  ArrayRemoveAll<unsigned_int>((Array *)(this + 0x1b0));
                    /* WARNING: Could not recover jumptable at 0x0008d4b6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(this + 0x1bc);
  return;
}

```

## target disasm
```
  0008d484: push {r4,r5,r7,lr}
  0008d486: add r7,sp,#0x8
  0008d488: mov r4,r0
  0008d48a: add.w r0,r0,#0x18c
  0008d48e: blx 0x0006f9d0
  0008d492: ldr r0,[0x0008d4b8]
  0008d494: add r0,pc
  0008d496: ldr r5,[r0,#0x0]
  0008d498: add.w r0,r4,#0x198
  0008d49c: blx r5
  0008d49e: add.w r0,r4,#0x1a4
  0008d4a2: blx r5
  0008d4a4: add.w r0,r4,#0x1b0
  0008d4a8: blx 0x0006fee0
  0008d4ac: add.w r0,r4,#0x1bc
  0008d4b0: mov r1,r5
  0008d4b2: pop.w {r4,r5,r7,lr}
  0008d4b6: bx r1
```
