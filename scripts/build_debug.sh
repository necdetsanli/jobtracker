#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"


echo "[jobtracker] Build directory: ${BUILD_DIR}"

cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build "${BUILD_DIR}" --parallel

(
	cd "${BUILD_DIR}"
	ctest --output-on-failure
)

echo "[jobtracker] Build + tests OK ✅"
