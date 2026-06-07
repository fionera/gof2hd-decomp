# Globals::reportLeaderboards
elf 0xe4b90 body 26
Sig: undefined __stdcall reportLeaderboards(void)

## decompile
```c

/* Globals::reportLeaderboards() */

void Globals::reportLeaderboards(void)

{
  undefined4 uVar1;
  
  uVar1 = Status::getKills((Status *)**(undefined4 **)(DAT_000f4bac + 0xf4b9a));
  **(undefined4 **)(DAT_000f4bb0 + 0xf4ba6) = uVar1;
  return;
}

```

## target disasm
```
  000f4b90: push {r7,lr}
  000f4b92: mov r7,sp
  000f4b94: ldr r0,[0x000f4bac]
  000f4b96: add r0,pc
  000f4b98: ldr r0,[r0,#0x0]
  000f4b9a: ldr r0,[r0,#0x0]
  000f4b9c: blx 0x00074a4c
  000f4ba0: ldr r1,[0x000f4bb0]
  000f4ba2: add r1,pc
  000f4ba4: ldr r1,[r1,#0x0]
  000f4ba6: str r0,[r1,#0x0]
  000f4ba8: pop {r7,pc}
```
