# EaseInOutMatrix::GetMinValue
elf 0x6edf2 body 12
Sig: undefined __stdcall GetMinValue(void)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::GetMinValue() */

void AbyssEngine::EaseInOutMatrix::GetMinValue(void)

{
  __aeabi_memcpy();
  return;
}

```

## target disasm
```
  0007edf2: push {r7,lr}
  0007edf4: mov r7,sp
  0007edf6: movs r2,#0x3c
  0007edf8: blx 0x0006f1e4
  0007edfc: pop {r7,pc}
```
