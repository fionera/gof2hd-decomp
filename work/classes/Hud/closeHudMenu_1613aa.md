# Hud::closeHudMenu
elf 0x1613aa body 38
Sig: undefined __stdcall closeHudMenu(void)

## decompile
```c

/* Hud::closeHudMenu() */

void Hud::closeHudMenu(void)

{
  int in_r0;
  void *pvVar1;
  
  if (*(Array **)(in_r0 + 0x18) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(in_r0 + 0x18));
    if (*(Array<TouchButton*> **)(in_r0 + 0x18) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(in_r0 + 0x18));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(in_r0 + 0x18) = 0;
  }
  *(undefined1 *)(in_r0 + 0x282) = 0;
  return;
}

```

## target disasm
```
  001713aa: push {r4,r6,r7,lr}
  001713ac: add r7,sp,#0x8
  001713ae: mov r4,r0
  001713b0: ldr r0,[r0,#0x18]
  001713b2: cbz r0,0x001713c8
  001713b4: blx 0x00075328
  001713b8: ldr r0,[r4,#0x18]
  001713ba: cbz r0,0x001713c4
  001713bc: blx 0x00075334
  001713c0: blx 0x0006eb48
  001713c4: movs r0,#0x0
  001713c6: str r0,[r4,#0x18]
  001713c8: movs r0,#0x0
  001713ca: strb.w r0,[r4,#0x282]
  001713ce: pop {r4,r6,r7,pc}
```
