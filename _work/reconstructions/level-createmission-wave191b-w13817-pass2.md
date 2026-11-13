# `Level::createMission` — w13817 pass 2 note (raid guns / case-4 alias)

On master `60ac5607f` the candidate (raid `guns` after the stream-out `if`,
drop the case-4 `enemies` alias) measures:

| | before | candidate |
|---|---:|---:|
| raw | **3,373** | 4,797 (+1,424) |
| aligned | **463** | 462 (-1) |
| size | 17,172 | **17,168/17,172** |

Size-fatal (the dropped alias costs 4 bytes), so the code change was reverted
and kept as this note.
