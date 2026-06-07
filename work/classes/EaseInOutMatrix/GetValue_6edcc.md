# EaseInOutMatrix::GetValue
elf 0x6edcc body 14
Sig: undefined __stdcall GetValue(void)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::GetValue() */

void AbyssEngine::EaseInOutMatrix::GetValue(void)

{
  __aeabi_memcpy();
  return;
}

```

## target disasm
```
  0007edcc: push {r7,lr}
  0007edce: mov r7,sp
  0007edd0: adds r1,#0x78
  0007edd2: movs r2,#0x3c
  0007edd4: blx 0x0006f1e4
  0007edd8: pop {r7,pc}
```
