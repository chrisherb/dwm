#!/bin/bash

# CPU Load (average over 1 minute)
cpu_load=$(awk '{printf "%s%%", $1}' /proc/loadavg)

# RAM Usage (free and used in GiB, 1 decimal)
read used total < <(free -m | awk '/^Mem:/ {print $3, $2}')
used_gb=$(awk "BEGIN {printf \"%.1f\", $used/1024}")
mem_info="${used_gb} GB"

# GPU Load (NVIDIA - needs nvidia-smi)
if command -v nvidia-smi >/dev/null 2>&1; then
gpu_load=$(nvidia-smi --query-gpu=utilization.gpu,memory.used,memory.total --format=csv,noheader,nounits | \
    awk -F', ' '{printf "%s%%", $1}')
else
gpu_load="N/A"
fi

# Date and Time
datetime=$(date '+%d.%m. %H:%M')

if command -v amixer >/dev/null 2>&1; then
    volume=$(amixer get Master | awk -F'[][%]' '/%/ {print $2; exit}')
    mute=$(amixer get Master | grep -q '\[off\]' && echo "MUTE" || echo "")
    if [ "$mute" = "MUTE" ]; then
	volume="MUTE"
    else
	volume="${volume}%"
    fi
else
    volume="N/A"
fi

# Combine all info, separating lines with " | "
status=" CPU: $cpu_load | GPU: $gpu_load | RAM: $mem_info | VOL: $volume | $datetime "

xsetroot -name "$status"
