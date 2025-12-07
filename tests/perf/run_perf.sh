#!/bin/bash

# Performance benchmark: rb::Tree vs std::set

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

if [ $# -ne 2 ]; then
    echo -e "${RED}ERROR: Please provide paths to both benchmark binaries${NC}"
    echo "Usage: $0 <rbtree_bench> <stdset_bench>"
    exit 1
fi

RBTREE_BIN="$1"
STDSET_BIN="$2"

if [ ! -f "$RBTREE_BIN" ]; then
    echo -e "${RED}ERROR: $RBTREE_BIN not found!${NC}"
    exit 1
fi

if [ ! -f "$STDSET_BIN" ]; then
    echo -e "${RED}ERROR: $STDSET_BIN not found!${NC}"
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

    rb_time=$("$RBTREE_BIN" < "$dat_file" 2>/dev/null)
    std_time=$("$STDSET_BIN" < "$dat_file" 2>/dev/null)

    if [ "$std_time" -gt 0 ] 2>/dev/null; then
        ratio=$(awk -v rb="$rb_time" -v std="$std_time" 'BEGIN {printf "%.2f", rb/std}')
    else
        ratio="N/A"
    fi

    color=$RED
    if awk -v r="$ratio" 'BEGIN { exit !(r < 1.2) }' 2>/dev/null; then
        color=$GREEN
    elif awk -v r="$ratio" 'BEGIN { exit !(r < 2.0) }' 2>/dev/null; then
        color=$YELLOW
    fi

    printf "%-10s %10s μs %10s μs ${color}%9sx${NC}\n" "$test_id" "$rb_time" "$std_time" "$ratio"
done

echo "========================================================"
echo -e "${CYAN}Legend: ${GREEN}< 1.2x = Excellent${NC} | ${YELLOW}1.2-2.0x = Good${NC} | ${RED}> 2.0x = Slow${NC}"
