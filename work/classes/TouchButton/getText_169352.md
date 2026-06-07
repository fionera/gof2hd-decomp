# TouchButton::getText
elf 0x169352 body 14
Sig: undefined __stdcall getText(void)

## decompile
```c

/* TouchButton::getText() */

void TouchButton::getText(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0xc),false);
  return;
}

```

## target disasm
```
  00179352: push {r7,lr}
  00179354: mov r7,sp
  00179356: adds r1,#0xc
  00179358: movs r2,#0x0
  0017935a: blx 0x0006f028
  0017935e: pop {r7,pc}
```
