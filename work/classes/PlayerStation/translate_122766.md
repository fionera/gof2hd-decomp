# PlayerStation::translate
elf 0x122766 body 10
Sig: undefined __stdcall translate(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerStation::translate(float, float, float) */

void PlayerStation::translate(float param_1,float param_2,float param_3)

{
  int in_r0;
  
  (*(code *)(DAT_001abc84 + 0x1abc88))(*(undefined4 *)(in_r0 + 0x140));
  return;
}

```

## target disasm
```
  00132766: ldr.w r0,[r0,#0x140]
  0013276a: b.w 0x001abc78
  001abc78: bx pc
```
