#!/usr/bin/env bash
# run_all.sh — fan out codex workers over a context index, capped at MAX concurrent.
#   CTX=work/<batch>_ctx.tsv  MAX=14  bash work/run_all.sh
CTX="${CTX:-work/tranche_ctx.tsv}"; MAX="${MAX:-12}"
cd /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
n=0
while IFS=$'\t' read -r name vaddr nb; do
  [ -z "$name" ] && continue
  while [ "$(jobs -rp | wc -l)" -ge "$MAX" ]; do sleep 2; done
  echo "launch $name ($vaddr/$nb)"
  bash work/run_worker.sh "$name" "$vaddr" "$nb" </dev/null >"work/results/$name.log" 2>&1 &
  n=$((n+1))
done < "$CTX"
wait
echo "ALL $n WORKERS DONE"
