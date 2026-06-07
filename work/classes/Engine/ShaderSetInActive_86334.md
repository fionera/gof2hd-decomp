# Engine::ShaderSetInActive
elf 0x86334 body 22
Sig: undefined __thiscall ShaderSetInActive(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ShaderSetInActive() */

void __thiscall AbyssEngine::Engine::ShaderSetInActive(Engine *this)

{
                    /* WARNING: Could not recover jumptable at 0x00096348. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(*(int *)(this + 0x514) + **(int **)(DAT_0009634c + 0x9633e) * 4) + 0x10))
            ();
  return;
}

```

## target disasm
```
  00096334: ldr r1,[0x0009634c]
  00096336: ldr.w r0,[r0,#0x514]
  0009633a: add r1,pc
  0009633c: ldr r1,[r1,#0x0]
  0009633e: ldr r1,[r1,#0x0]
  00096340: ldr.w r0,[r0,r1,lsl #0x2]
  00096344: ldr r1,[r0,#0x0]
  00096346: ldr r1,[r1,#0x10]
  00096348: bx r1
```
