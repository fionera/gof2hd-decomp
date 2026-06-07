# AESoundRessource::play
elf 0x7fee4 body 6
Sig: undefined __thiscall play(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::play(int) */

void __thiscall AbyssEngine::AESoundRessource::play(AESoundRessource *this,int param_1)

{
  (*(code *)(DAT_001ab494 + 0x1ab498))(this,param_1,0);
  return;
}

```

## target disasm
```
  0008fee4: movs r2,#0x0
  0008fee6: b.w 0x001ab488
```
