# PlayerFighter::setPosition
elf 0xdcb9a body 28
Sig: undefined __thiscall setPosition(PlayerFighter * this, Vector * param_1)

## decompile
```c

/* PlayerFighter::setPosition(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerFighter::setPosition(PlayerFighter *this,Vector *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x000ecbb4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(int *)this + 0x48))
            (this,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  000ecb9a: push {r7,lr}
  000ecb9c: mov r7,sp
  000ecb9e: ldr.w lr,[r0,#0x0]
  000ecba2: ldrd r12,r2,[r1,#0x0]
  000ecba6: ldr r3,[r1,#0x8]
  000ecba8: ldr.w lr,[lr,#0x48]
  000ecbac: mov r1,r12
  000ecbae: mov r12,lr
  000ecbb0: pop.w {r7,lr}
  000ecbb4: bx r12
```
