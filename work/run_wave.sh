#!/usr/bin/env bash
# run_wave.sh — launch a class-worker per line in work/wave.tsv, in parallel. Each class-worker
# fans out its own method sub-workers, so we cap the number of concurrent CLASS-workers low.
cd /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
MAXC="${MAXC:-3}"
while IFS=$'\t' read -r dir klass; do
  [ -z "$dir" ] && continue
  while [ "$(jobs -rp | wc -l)" -ge "$MAXC" ]; do sleep 5; done
  mkdir -p "work/classes/$dir/results"
  echo "launch class-worker: $dir ($klass)"
  bash work/run_class.sh "$dir" </dev/null >"work/classes/$dir/results/_launch.log" 2>&1 &
done < work/wave.tsv
wait
echo "WAVE DONE"
