# ApplicationManager::GetElapsedTimeMillis
elf 0x827dc body 18
Sig: undefined __stdcall GetElapsedTimeMillis(void)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetElapsedTimeMillis() */

undefined8 AbyssEngine::ApplicationManager::GetElapsedTimeMillis(void)

{
  int in_r0;
  
  return CONCAT44((*(int *)(in_r0 + 0x74) - *(int *)(in_r0 + 0x7c)) -
                  (uint)(*(uint *)(in_r0 + 0x70) < *(uint *)(in_r0 + 0x78)),
                  *(uint *)(in_r0 + 0x70) - *(uint *)(in_r0 + 0x78));
}

```

## target disasm
```
  000927dc: ldrd r12,r2,[r0,#0x70]
  000927e0: ldrd r3,r1,[r0,#0x78]
  000927e4: subs.w r0,r12,r3
  000927e8: sbc.w r1,r2,r1
  000927ec: bx lr
```
