#!/usr/bin/env bash
set -euo pipefail

# Resolve project root relative to this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"

echo "[jobtracker] Build directory: ${BUILD_DIR}"

# 1) Configure (Debug)
cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Debug

# 2) Build
cmake --build "${BUILD_DIR}" --parallel

# 3) Run tests
(
	cd "${BUILD_DIR}"
	ctest --output-on-failure
)

echo "[jobtracker] Build + tests OK ✅"
