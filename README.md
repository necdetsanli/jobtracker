# jobtracker

`jobtracker` is a small, test-driven C++20 command-line tool for tracking job applications.

It stores applications in a SQLite database, exposes a simple CLI to add/list/query them, and provides an import layer to bulk-load applications from CSV files. The codebase is structured as a set of small, focused libraries (`core`, `storage`, `import`, `cli`) to demonstrate clean layering and testability.

---

## Features

- Track job applications in a local SQLite database
- Add applications from the command line
- List all stored applications
- Show basic statistics by status (e.g. `applied`, `interview`, `offer`)
- Import applications from CSV files (e.g. exports from spreadsheets or job boards)
- Test-driven core with Catch2
- CMake-based build and a helper script for build+test

---

## Architecture

High-level layout:

- **Core (`jobtracker_core`)**
  - `Application` struct: data model for a job application
  - `IApplicationRepository`: abstraction for persistence
  - `JobTracker`: domain service that owns business rules (defaults, statistics)

- **Storage (`jobtracker_storage_sqlite`)**
  - `SqliteDatabase`: RAII wrapper around `sqlite3*` handles
  - `SqliteApplicationRepository`: SQLite implementation of `IApplicationRepository`

- **Import (`jobtracker_import`)**
  - `IImportSource`: abstraction for external sources (CSV, email, job boards, …)
  - `ImportService`: coordinates `IImportSource` → `JobTracker`
  - `CsvImportSource`: reads applications from a CSV file

- **CLI (`jobtracker_cli_lib` and `jobtracker_cli`)**
  - `CommandLineOptions` + `parse_arguments()`: testable argument parsing
  - `jobtracker_cli`: executable wiring CLI → JobTracker → SQLite/Import

Directory structure (simplified):

```text
.
├── src
│   ├── core
│   ├── storage
│   ├── import
│   ├── cli
│   └── util
├── tests
│   ├── core
│   ├── storage
│   ├── import
│   ├── cli
│   └── util
├── scripts
│   └── build_debug.sh
|   └── build_release.sh
└── data
    └── jobtracker.db   (created at runtime)
```

---

## Requirements

- C++20-capable compiler (GCC or Clang)
- CMake ≥ 3.16
- Ninja (recommended as CMake generator)
- SQLite3 development files
- Catch2 v3 (for tests)

On Debian/Ubuntu, something like:

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  ninja-build \
  libsqlite3-dev \
  catch2
```

(Adjust package names as needed for your distribution.)

---

## Building and running tests

### Using the helper script (Debug + Ninja)

From the project root:

```bash
./scripts/build_debug.sh
```

This script will:

1. Configure a Debug build under `build/` using the Ninja generator
2. Build all libraries, executables and tests
3. Run tests via `ctest --output-on-failure`

### Manual Debug build with Ninja

If you prefer to run CMake manually:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
cd build
ctest --output-on-failure
```

If you omit `-G Ninja`, CMake will use its default generator for your platform (e.g. Unix Makefiles on Linux).

### Release build

There is also a helper script for a Release build:

```bash
./scripts/build_release.sh
```

This will:

1. Configure a Release build under `build-release/` using the Ninja generator
2. Build all targets
3. Run tests in the Release tree (failures do not cause the script to exit with an error)


After a successful build, the CLI binary is typically located at:

- Debug build: `build/bin/jobtracker_cli`
- Release build: `build-release/bin/jobtracker_cli`

(If you use a different build directory, replace `build` / `build-release` accordingly.)

---

## Database location

By default, the CLI uses a SQLite database at:

```text
data/jobtracker.db
```

relative to the **current working directory**. Make sure the `data/` folder exists:

```bash
mkdir -p data
```

You can override the database path with `--db PATH` on any command.

---

## CLI usage

Basic syntax:

```bash
jobtracker_cli <command> [options]
```

Available commands:

- `list` – list all applications
- `add` – add a new application
- `stats` – show statistics by status
- `import-csv` – import applications from a CSV file
- `help` – show usage

Below, assume the binary lives at `build/src/jobtracker_cli`. If it is under `build/bin/`, simply adjust the path.

### Help

```bash
./build/src/jobtracker_cli help
```

### Add a new application

```bash
./build/src/jobtracker_cli add \
  --company "ACME" \
  --position "C++ Developer" \
  --location "Remote" \
  --source "linkedin" \
  --status "applied" \
  --notes "Applied via LinkedIn"
```

Required for `add`:

- `--company`
- `--position`

Optional:

- `--location`
- `--source`
- `--status` (defaults to `"applied"` if not set by the domain layer)
- `--notes`

You can also point to a different database:

```bash
./build/src/jobtracker_cli add \
  --db data/my-jobtracker.db \
  --company "Beta" \
  --position "DevOps Engineer"
```

### List applications

```bash
./build/src/jobtracker_cli list
```

Example output:

```text
#1 | ACME | C++ Developer | applied   | 2025-01-01
#2 | Beta | DevOps Engineer | interview | 2025-01-02
```

If there are no applications yet:

```text
No applications found.
Use 'add' or 'import-csv' to create applications.
```

### Show statistics

```bash
./build/src/jobtracker_cli stats
```

Example output:

```text
Applications by status:
  applied: 3
  interview: 1
  offer: 0
```

If there are no applications:

```text
No applications found.
There are no statistics to display yet.
```

---

## CSV import

You can bulk-import applications from a CSV file using the `import-csv` command.

### Expected CSV format

The CSV must have a header row. Column names are matched case-insensitively. Supported columns:

- `company`
- `position`
- `location`
- `source`
- `status`
- `applied_date`
- `last_update`
- `notes`

Unknown columns are ignored. Rows that have both `company` and `position` empty are skipped.

Example `data/import.csv`:

```csv
company,position,location,source,status,applied_date,last_update,notes
ACME,C++ Developer,Remote,linkedin,applied,2025-01-01,2025-01-01,Imported from CSV
Beta,DevOps Engineer,Berlin,email,interview,2025-01-02,2025-01-03,Imported from CSV
```

### Running the import

```bash
mkdir -p data

./build/src/jobtracker_cli import-csv \
  --csv data/import.csv \
  --db data/jobtracker.db
```

Example output:

```text
Imported 2 applications out of 2 rows from CSV.
```

Edge cases:

- If `--csv` is missing:

  ```text
  Error: --csv PATH is required for 'import-csv'.
  ```

- If the file does not exist:

  ```text
  Error: CSV file 'data/missing.csv' does not exist.
  Please check the path and try again.
  ```

- If the file is empty or contains only a header row:

  ```text
  No rows found in CSV file 'data/import.csv'.
  Make sure the file is not empty and has a header row.
  ```

- If no valid rows could be mapped to applications:

  ```text
  No applications were imported from 'data/import.csv'.
  Check that each row has at least a company or position.
  ```

---

## Running tests

All tests are built into a single test runner (using Catch2). After building:

```bash
cd build
ctest --output-on-failure
```

or simply:

```bash
./scripts/build_debug.sh
```

which already runs `ctest` for you.

Tests cover:

- String utilities and date/time helpers
- Core `JobTracker` behavior and statistics
- SQLite repository behavior (CRUD + stats)
- Import layer (`ImportService`, `FakeImportSource`, `CsvImportSource`)
- CLI argument parsing (`parse_arguments()`)

---

## Development notes

- Language: C++20
- Build system: CMake
- Testing: Catch2 v3
- Storage: SQLite3
- Layout:
  - `src/core` – domain model and business logic
  - `src/storage` – persistence implementations
  - `src/import` – import sources and orchestration
  - `src/cli` – command-line interface and argument parsing
  - `src/util` – small utility helpers
  - `tests` – mirrored structure with unit tests
  - `scripts` – helper scripts (build, etc.)
- Code is written with testability in mind; most logic lives in libraries, not in `main()`.

### Cross-platform notes

The project is designed to be portable across platforms:

- Uses only standard C++ and the C++ standard library for core logic
- Avoids POSIX-only APIs; platform-specific code paths (e.g. localtime_r vs. localtime_s) are guarded with preprocessor checks
- Compiler flags are configured conditionally in CMake (MSVC vs. GCC/Clang)

Planned:

- Windows support using MSVC + CMake + Ninja
- Optional vcpkg integration for dependencies such as SQLite3 and Catch2

---

## Possible future work

Some potential extensions:

- Additional import sources:
  - IMAP/Gmail-based importer parsing job application emails
  - LinkedIn or other job board exports
- Alternative storage backends:
  - PostgreSQL/MySQL repository implementations
- Interactive TUI or GUI (e.g. Qt-based front-end)
- More advanced filters in the CLI (by status, date range, company, …)