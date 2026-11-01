#!/bin/bash
# Remove a finished worker worktree and its branch. Run only after its work is harvested.
# usage: tools/remove_worker.sh <wid>
set -e
MAIN=$(cd "$(dirname "$0")/.." && pwd)
w=$1
d=/home/debian/GalaxyOnFire2/wt/$w
git -C "$MAIN" worktree remove --force "$d" 2>/dev/null || true
git -C "$MAIN" branch -D "wave87-$w" 2>/dev/null || true
git -C "$MAIN" worktree prune
echo "removed $d"
