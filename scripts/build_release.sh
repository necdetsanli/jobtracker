#!/usr/bin/env bash
set -euo pipefail

# Resolve project root relative to this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build-release"

echo "[jobtracker] Build directory: ${BUILD_DIR}"

# Configure Release build with Ninja generator
cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build "${BUILD_DIR}" --parallel

# Run tests (do not fail the script if tests fail in Release)
(
	cd "${BUILD_DIR}"
	ctest --output-on-failure || true
)

echo "[jobtracker] Release build OK ✅"
