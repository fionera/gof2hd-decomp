# FBOContainer::EndCapture
elf 0x7fa18 body 16
Sig: undefined __stdcall EndCapture(void)

## decompile
```c

/* AbyssEngine::FBOContainer::EndCapture() */

void AbyssEngine::FBOContainer::EndCapture(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab474 + 0x1ab478))(0x8d40,*(undefined4 *)(*(int *)(in_r0 + 0x14) + 0x40c));
  return;
}

```

## target disasm
```
  0008fa18: ldr r0,[r0,#0x14]
  0008fa1a: ldr.w r1,[r0,#0x40c]
  0008fa1e: movw r0,#0x8d40
  0008fa22: b.w 0x001ab468
  001ab468: bx pc
```
