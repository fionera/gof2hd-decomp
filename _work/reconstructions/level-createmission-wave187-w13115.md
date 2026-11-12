# `Level::createMission` — wave 187 / w13115 (uncommitted harvest)

The worker's final pass remaps the `case4Tail` `offset[1]` values across cases
0/1/2: case 0 gets `0.0f` (was `-3000.0f`), case 1 gets `-3000.0f` (was
`-7000.0f`), case 2 gets `-7000.0f` (was `0.0f`). This is a cyclic shift of the
three `offset[1]` constants; it matches the original literal-pool order at
`0xb15bc..0xb15d8` (`-3000, 20000, -50000, -7000, 10000, -30000, 25000,
-36000`) and is size-exact.

Measured on the wave-187 combined tree: raw **3,517 -> 3,510** (-7), `.text`
74,243 -> 74,236, `.ARM.extab` 647 unchanged, `.rel.plt` byte-identical.
The worker hit the timeout before committing; the diff was harvested from
`wt/w13115` and landed here.
