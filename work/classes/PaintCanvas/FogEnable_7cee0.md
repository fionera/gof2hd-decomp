# PaintCanvas::FogEnable
elf 0x7cee0 body 58
Sig: undefined __stdcall FogEnable(bool param_1, FogMode param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::FogEnable(bool, AbyssEngine::FogMode) */

void AbyssEngine::PaintCanvas::FogEnable(int param_1,int param_2,int param_3)

{
  *(int *)(param_1 + 500) = param_3;
  if (param_3 == 0) {
    if (**(char **)(DAT_0008cf1c + 0x8cef4) == '\0') {
      if (param_2 == 0) {
        glDisable(0xb60);
      }
      else {
        glEnable();
      }
    }
    else {
      **(undefined1 **)(DAT_0008cf20 + 0x8cefe) = (char)param_2;
    }
    param_2 = 0;
  }
  *(char *)(param_1 + 0x1f1) = (char)param_2;
  return;
}

```

## target disasm
```
  0008cee0: push {r4,r6,r7,lr}
  0008cee2: add r7,sp,#0x8
  0008cee4: mov r4,r0
  0008cee6: cmp r2,#0x0
  0008cee8: str.w r2,[r0,#0x1f4]
  0008ceec: bne 0x0008cf14
  0008ceee: ldr r0,[0x0008cf1c]
  0008cef0: add r0,pc
  0008cef2: ldr r0,[r0,#0x0]
  0008cef4: ldrb r0,[r0,#0x0]
  0008cef6: cbz r0,0x0008cf02
  0008cef8: ldr r0,[0x0008cf20]
  0008cefa: add r0,pc
  0008cefc: ldr r0,[r0,#0x0]
  0008cefe: strb r1,[r0,#0x0]
  0008cf00: b 0x0008cf12
  0008cf02: mov.w r0,#0xb60
  0008cf06: cbz r1,0x0008cf0e
  0008cf08: blx 0x0006edb8
  0008cf0c: b 0x0008cf12
  0008cf0e: blx 0x0006edc4
  0008cf12: movs r1,#0x0
  0008cf14: strb.w r1,[r4,#0x1f1]
  0008cf18: pop {r4,r6,r7,pc}
```
