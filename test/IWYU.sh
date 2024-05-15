#!/bin/bash

# Variables
declare -a source_files=(
  "core.cpp"
  "app.cpp"
)

# Source code check
echo ""
echo "======================================================"
echo "     IWYU Checks"
echo "======================================================"
echo ""
for source in "${source_files[@]}"; do
  include-what-you-use -std=c++20 -Iinclude src/"$source"

  echo ""
  echo "--------------------------------------------------"
  echo ""
done