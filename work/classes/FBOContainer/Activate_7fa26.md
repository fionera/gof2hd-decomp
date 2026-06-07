# FBOContainer::Activate
elf 0x7fa26 body 10
Sig: undefined __stdcall Activate(void)

## decompile
```c

/* AbyssEngine::FBOContainer::Activate() */

void AbyssEngine::FBOContainer::Activate(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab484 + 0x1ab488))(0xde1,*(undefined4 *)(in_r0 + 4));
  return;
}

```

## target disasm
```
  0008fa26: ldr r1,[r0,#0x4]
  0008fa28: movw r0,#0xde1
  0008fa2c: b.w 0x001ab478
```
