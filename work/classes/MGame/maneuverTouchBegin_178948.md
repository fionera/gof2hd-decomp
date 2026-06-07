# MGame::maneuverTouchBegin
elf 0x178948 body 18
Sig: undefined __stdcall maneuverTouchBegin(int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::maneuverTouchBegin(int, int, void*) */

void MGame::maneuverTouchBegin(int param_1,int param_2,void *param_3)

{
  *(undefined1 *)(param_1 + 0x17c) = 1;
  *(int *)(param_1 + 0x180) = param_2;
  *(void **)(param_1 + 0x184) = param_3;
  *(undefined4 *)(param_1 + 0x178) = 0;
  return;
}

```

## target disasm
```
  00188948: movs r3,#0x1
  0018894a: strb.w r3,[r0,#0x17c]
  0018894e: strd r1,r2,[r0,#0x180]
  00188952: movs r1,#0x0
  00188954: str.w r1,[r0,#0x178]
  00188958: bx lr
```
