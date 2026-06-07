# EaseInOutMatrix::GetMaxValue
elf 0x6edfe body 14
Sig: undefined __stdcall GetMaxValue(void)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::GetMaxValue() */

void AbyssEngine::EaseInOutMatrix::GetMaxValue(void)

{
  __aeabi_memcpy();
  return;
}

```

## target disasm
```
  0007edfe: push {r7,lr}
  0007ee00: mov r7,sp
  0007ee02: adds r1,#0xb4
  0007ee04: movs r2,#0x3c
  0007ee06: blx 0x0006f1e4
  0007ee0a: pop {r7,pc}
```
