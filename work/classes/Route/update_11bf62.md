# Route::update
elf 0x11bf62 body 14
Sig: undefined __thiscall update(Route * this, Vector * param_1)

## decompile
```c

/* Route::update(AbyssEngine::AEMath::Vector const&) */

void __thiscall Route::update(Route *this,Vector *param_1)

{
  (*(code *)(DAT_001ac354 + 0x1ac358))
            (this,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  0012bf62: ldrd r12,r2,[r1,#0x0]
  0012bf66: ldr r3,[r1,#0x8]
  0012bf68: mov r1,r12
  0012bf6a: b.w 0x001ac348
  001ac348: bx pc
```
