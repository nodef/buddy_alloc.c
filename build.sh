#!/usr/bin/env bash
# Fetch the latest version of the library
fetch() {
if [ -f "buddy_alloc.h" ]; then return; fi
URL="https://github.com/spaskalev/buddy_alloc/raw/refs/heads/main/buddy_alloc.h"

# Download the release
echo "Downloading buddy_alloc.h from $URL ..."
curl -L "$URL" -o "buddy_alloc.h"
echo ""
}


# Test the project
test() {
echo "Running 01-basic.c ..."
clang -I. -o 01.exe examples/01-basic.c         && ./01 && echo -e "\n"
echo "Running 02-dynamic.c ..."
clang -I. -o 02.exe examples/02-dynamic.c       && ./02 && echo -e "\n"
echo "Running 03-walk.c ..."
clang -I. -o 03.exe examples/03-walk.c          && ./03 && echo -e "\n"
echo "Running 04-fragmentation.c ..."
clang -I. -o 04.exe examples/04-fragmentation.c && ./04 && echo -e "\n"
}


# Main script
if [[ "$1" == "test" ]]; then test
elif [[ "$1" == "fetch" ]]; then fetch
else echo "Usage: $0 {fetch|test}"; fi
