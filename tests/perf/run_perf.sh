#!/bin/bash

# Performance benchmark: rb::Tree vs std::set

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

if [ $# -eq 0 ]; then
    echo -e "${RED}ERROR: Please provide path to benchmark binary${NC}"
    echo "Usage: $0 <path_to_benchmark>"
    exit 1
fi

BENCH_BIN="$1"

if [ ! -f "$BENCH_BIN" ]; then
    echo -e "${RED}ERROR: $BENCH_BIN not found!${NC}"
    exit 1
fi

E2E_DIR="../end2end"

if [ ! -d "$E2E_DIR" ]; then
    echo -e "${RED}ERROR: end2end directory not found at $E2E_DIR${NC}"
    exit 1
fi

echo -e "${CYAN}Performance Benchmark: rb::Tree vs std::set${NC}"
echo "========================================================"
printf "%-10s %12s %12s %10s\n" "Test" "rb::Tree" "std::set" "Ratio"
echo "--------------------------------------------------------"

for dat_file in "$E2E_DIR"/*.dat; do
    test_id=$(basename "$dat_file" .dat)

    output=$("$BENCH_BIN" < "$dat_file" 2>/dev/null)

    rb_time=$(echo "$output" | grep "rb::Tree" | awk '{print $2}')
    std_time=$(echo "$output" | grep "std::set" | awk '{print $2}')
    ratio=$(echo "$output" | grep "Ratio" | awk '{print $2}')

    ratio_clean=$(echo "$ratio" | sed 's/x$//')

    color=$RED
    if awk -v r="$ratio_clean" 'BEGIN { exit !(r < 1.2) }'; then
        color=$GREEN
    elif awk -v r="$ratio_clean" 'BEGIN { exit !(r < 2.0) }'; then
        color=$YELLOW
    fi

    printf "%-10s %10s μs %10s μs ${color}%10s${NC}\n" "$test_id" "$rb_time" "$std_time" "$ratio"
done

echo "========================================================"
echo -e "${CYAN}Legend: ${GREEN}< 1.2x = Excellent${NC} | ${YELLOW}1.2-2.0x = Good${NC} | ${RED}> 2.0x = Slow${NC}"
