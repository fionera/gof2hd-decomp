# Engine::LightSetMaterialColorAlpha
elf 0x856b4 body 100
Sig: undefined __thiscall LightSetMaterialColorAlpha(Engine * this, float param_1)

## decompile
```c

/* AbyssEngine::Engine::LightSetMaterialColorAlpha(float) */

void __thiscall AbyssEngine::Engine::LightSetMaterialColorAlpha(Engine *this,float param_1)

{
  undefined4 *puVar1;
  undefined4 in_r1;
  code *UNRECOVERED_JUMPTABLE;
  
  if (this[0x424] != (Engine)0x0) {
    puVar1 = (undefined4 *)(DAT_00095718 + 0x956d2);
    *(undefined4 *)(this + 0x2b4) = in_r1;
    *(undefined4 *)(this + 0x488) = in_r1;
    UNRECOVERED_JUMPTABLE = (code *)*puVar1;
    (*UNRECOVERED_JUMPTABLE)(0x408,0x1200,this + 0x2a8);
    *(undefined4 *)(this + 0x2c4) = *(undefined4 *)(this + 0x488);
    (*UNRECOVERED_JUMPTABLE)(0x408,0x1202,this + 0x2b8);
    *(undefined4 *)(this + 0x2a4) = *(undefined4 *)(this + 0x488);
                    /* WARNING: Could not recover jumptable at 0x00095714. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(0x408,0x1201,this + 0x298);
    return;
  }
  return;
}

```

## target disasm
```
  000956b4: push {r4,r5,r7,lr}
  000956b6: add r7,sp,#0x8
  000956b8: mov r4,r0
  000956ba: ldrb.w r0,[r0,#0x424]
  000956be: cbz r0,0x00095716
  000956c0: ldr r0,[0x00095718]
  000956c2: vmov s0,r1
  000956c6: add.w r1,r4,#0x488
  000956ca: add.w r2,r4,#0x2a8
  000956ce: add r0,pc
  000956d0: vstr.32 s0,[r4,#0x2b4]
  000956d4: vstr.32 s0,[r1]
  000956d8: mov.w r1,#0x1200
  000956dc: ldr r5,[r0,#0x0]
  000956de: mov.w r0,#0x408
  000956e2: blx r5
  000956e4: ldr.w r0,[r4,#0x488]
  000956e8: add.w r2,r4,#0x2b8
  000956ec: str.w r0,[r4,#0x2c4]
  000956f0: mov.w r0,#0x408
  000956f4: movw r1,#0x1202
  000956f8: blx r5
  000956fa: ldr.w r0,[r4,#0x488]
  000956fe: add.w r2,r4,#0x298
  00095702: str.w r0,[r4,#0x2a4]
  00095706: mov.w r0,#0x408
  0009570a: movw r1,#0x1201
  0009570e: mov r3,r5
  00095710: pop.w {r4,r5,r7,lr}
  00095714: bx r3
  00095716: pop {r4,r5,r7,pc}
```
