# Layout::update
elf 0xd5152 body 86
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* Layout::update(int) */

void Layout::update(int param_1)

{
  int iVar1;
  int in_r1;
  
  if (*(int *)(param_1 + 0x3c4) != 0) {
    ChoiceWindow::update(*(int *)(param_1 + 0x3c4));
  }
  if ((*(char *)(param_1 + 0x2ec) != '\0') &&
     (iVar1 = *(int *)(param_1 + 0x3d0) + in_r1, *(int *)(param_1 + 0x3d0) = iVar1, 6999 < iVar1)) {
    *(undefined1 *)(param_1 + 0x2ec) = 0;
  }
  if ((*(char *)(param_1 + 0x400) != '\0') &&
     (iVar1 = *(int *)(param_1 + 0x408) + in_r1, *(int *)(param_1 + 0x408) = iVar1,
     *(int *)(param_1 + 0x40c) < iVar1)) {
    *(undefined4 *)(param_1 + 0x408) = 0;
    *(undefined1 *)(param_1 + 0x400) = 0;
  }
  return;
}

```

## target disasm
```
  000e5152: push {r4,r5,r7,lr}
  000e5154: add r7,sp,#0x8
  000e5156: mov r4,r0
  000e5158: ldr.w r0,[r0,#0x3c4]
  000e515c: mov r5,r1
  000e515e: cbz r0,0x000e5166
  000e5160: mov r1,r5
  000e5162: blx 0x00074eb4
  000e5166: ldrb.w r0,[r4,#0x2ec]
  000e516a: cbz r0,0x000e5184
  000e516c: ldr.w r0,[r4,#0x3d0]
  000e5170: movw r1,#0x1b58
  000e5174: add r0,r5
  000e5176: str.w r0,[r4,#0x3d0]
  000e517a: cmp r0,r1
  000e517c: itt ge
  000e517e: mov.ge r0,#0x0
  000e5180: strb.ge.w r0,[r4,#0x2ec]
  000e5184: ldrb.w r0,[r4,#0x400]
  000e5188: cbz r0,0x000e51a6
  000e518a: ldr.w r0,[r4,#0x408]
  000e518e: ldr.w r1,[r4,#0x40c]
  000e5192: add r0,r5
  000e5194: str.w r0,[r4,#0x408]
  000e5198: cmp r0,r1
  000e519a: ittt gt
  000e519c: mov.gt r0,#0x0
  000e519e: str.w.gt r0,[r4,#0x408]
  000e51a2: strb.gt.w r0,[r4,#0x400]
  000e51a6: pop {r4,r5,r7,pc}
```
