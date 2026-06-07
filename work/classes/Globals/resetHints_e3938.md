# Globals::resetHints
elf 0xe3938 body 38
Sig: undefined __stdcall resetHints(void)

## decompile
```c

/* Globals::resetHints() */

void Globals::resetHints(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar4 = *(undefined4 **)(DAT_000f3960 + 0xf3944);
  *puVar4 = 0;
  puVar4[1] = uVar1;
  puVar4[2] = uVar2;
  puVar4[3] = uVar3;
  *(undefined4 *)((int)puVar4 + 0x2b) = 0;
  *(undefined4 *)((int)puVar4 + 0x2f) = uVar1;
  *(undefined4 *)((int)puVar4 + 0x33) = uVar2;
  *(undefined4 *)((int)puVar4 + 0x37) = uVar3;
  puVar4[8] = 0;
  puVar4[9] = uVar1;
  puVar4[10] = uVar2;
  puVar4[0xb] = uVar3;
  puVar4[4] = 0;
  puVar4[5] = uVar1;
  puVar4[6] = uVar2;
  puVar4[7] = uVar3;
  return;
}

```

## target disasm
```
  000f3938: ldr r0,[0x000f3960]
  000f393a: vmov.i32 q8,#0x0
  000f393e: movs r1,#0x2b
  000f3940: add r0,pc
  000f3942: ldr r0,[r0,#0x0]
  000f3944: mov r2,r0
  000f3946: vst1.8 {d16,d17},[r2],r1
  000f394a: add.w r1,r0,#0x20
  000f394e: adds r0,#0x10
  000f3950: vst1.8 {d16,d17},[r2]
  000f3954: vst1.8 {d16,d17},[r1]
  000f3958: vst1.8 {d16,d17},[r0]
  000f395c: bx lr
```
