# TargetFollowCamera::getLocal
elf 0x15b3d4 body 16
Sig: undefined __stdcall getLocal(void)

## decompile
```c

/* TargetFollowCamera::getLocal() */

void TargetFollowCamera::getLocal(void)

{
  __aeabi_memcpy();
  return;
}

```

## target disasm
```
  0016b3d4: push {r7,lr}
  0016b3d6: mov r7,sp
  0016b3d8: add.w r1,r1,#0x13c
  0016b3dc: movs r2,#0x3c
  0016b3de: blx 0x0006f1e4
  0016b3e2: pop {r7,pc}
```
