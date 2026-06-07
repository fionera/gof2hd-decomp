# Achievements::resetNewMedals
elf 0x156f8e body 34
Sig: undefined __thiscall resetNewMedals(Achievements * this)

## decompile
```c

/* Achievements::resetNewMedals() */

void __thiscall Achievements::resetNewMedals(Achievements *this)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(this + 4);
  for (iVar2 = 0; iVar2 != 0x2d; iVar2 = iVar2 + 1) {
    *(undefined4 *)(iVar1 + iVar2 * 4) = 0;
  }
  this[0x18] = (Achievements)0x0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  return;
}

```

## target disasm
```
  00166f8e: ldr r1,[r0,#0x4]
  00166f90: movs r2,#0x0
  00166f92: movs r3,#0x0
  00166f94: b 0x00166f9c
  00166f96: str.w r2,[r1,r3,lsl #0x2]
  00166f9a: adds r3,#0x1
  00166f9c: cmp r3,#0x2d
  00166f9e: bne 0x00166f96
  00166fa0: movs r1,#0x0
  00166fa2: vmov.i32 q8,#0x0
  00166fa6: strb r1,[r0,#0x18]
  00166fa8: adds r0,#0x8
  00166faa: vst1.32 {d16,d17},[r0]
  00166fae: bx lr
```
