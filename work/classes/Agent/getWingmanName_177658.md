# Agent::getWingmanName
elf 0x177658 body 24
Sig: undefined __stdcall getWingmanName(int param_1)

## decompile
```c

/* Agent::getWingmanName(int) */

void Agent::getWingmanName(int param_1)

{
  String *in_r1;
  int in_r2;
  
  if (in_r2 == 1) {
    in_r1 = *(String **)(in_r1 + 0xc);
  }
  else if (in_r2 != 0) {
    in_r1 = *(String **)(in_r1 + 0x10);
  }
  AbyssEngine::String::String((String *)param_1,in_r1,false);
  return;
}

```

## target disasm
```
  00187658: cmp r2,#0x1
  0018765a: beq 0x00187662
  0018765c: cbz r2,0x00187664
  0018765e: ldr r1,[r1,#0x10]
  00187660: b 0x00187664
  00187662: ldr r1,[r1,#0xc]
  00187664: push {r7,lr}
  00187666: mov r7,sp
  00187668: movs r2,#0x0
  0018766a: blx 0x0006f028
  0018766e: pop {r7,pc}
```
