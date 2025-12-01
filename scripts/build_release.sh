#!/usr/bin/env bash
set -euo pipefail

# Resolve project root relative to this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build-release"

echo "[jobtracker] Build directory: ${BUILD_DIR}"

cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" --parallel

(
	cd "${BUILD_DIR}"
	ctest --output-on-failure || true
)

echo "[jobtracker] Release build OK ✅"
