# JobTracker C++ Coding Style Guide

This document describes the coding conventions used in the **jobtracker** repository.
All new code should follow these guidelines. When editing existing code, prefer to
adapt it to this style where reasonable.

---

## 1. Language and general rules

- **Language:** All identifiers, comments, log messages, error messages and docs **must be in English**.
- **Standard:** C++20 (as configured in CMake).
- **Headers:** Use `#pragma once` for include guards.
- **No `using namespace std;`:** Always qualify standard library names (`std::string`, `std::vector`, etc.).
- **RAII first:** Prefer RAII wrappers and value types over raw resource management (`new`/`delete`, manual `close()` calls, etc.).
- **Exceptions:** Use exceptions for truly exceptional / fatal conditions (e.g. database cannot be opened). For “normal” control flow, prefer return values (`bool`, `std::optional`, etc.).

---

## 2. File naming and layout

### 2.1 Filenames

- All filenames are **lowercase** and use **snake_case**:
  - `application.h`, `application.cpp`
  - `job_tracker.h`, `job_tracker.cpp`
  - `sqlite_database.h`, `sqlite_database.cpp`
- Test files mirror the module structure under `tests/`:
  - `src/util/string_utils.cpp` → `tests/util/test_string_utils.cpp`
  - `src/import/remote_csv_import_source.cpp` → `tests/import/test_remote_csv_import_source.cpp`

### 2.2 Directory structure (high level)

- `src/core` — Domain model and business logic (e.g. `Application`, `JobTracker`).
- `src/storage` — Persistence layer (e.g. SQLite repository).
- `src/import` — Import sources (CSV, IMAP, remote CSV, etc.).
- `src/cli` — Command-line interface (argument parsing + main executable).
- `src/util` — Small, generic helpers (`string_utils`, `datetime`, config parsing, etc.).
- `tests` — Unit tests, mirroring the `src` layout.
- `scripts` — Build and tooling scripts.
- `data` — Local data (DBs, CSVs, configs). This directory is **not** committed; see `.gitignore`.

---

## 3. Formatting

We rely on `clang-format` for most formatting decisions. The rules below capture the intent.

### 3.1 Indentation and braces

- **Indentation:** One **tab** per indentation level.
- **Braces:** Opening brace is on the **next line**.

**Example – functions:**

```cpp
int add(int a, int b)
{
	return a + b;
}
```

#### 3.2 Examples – classes / structs

    class JobTracker
    {
    public:
        JobTracker(IApplicationRepository &repository);

        void add_application(const Application &app);

    private:
        IApplicationRepository &repository_;
    };

#### 3.3 Examples – control flow

    if (value > 0)
    {
        do_something();
    }
    else
    {
        do_something_else();
    }

#### 3.4 Line length

- Try to keep lines around **100–120 characters**.
- Break long expressions or parameter lists across multiple lines, aligning them in a readable way.

#### 3.5 Includes

- Use `#pragma once` on top of every header.
- Include order:
  1. Matching header (if any)
  2. C / C++ standard library headers
  3. Third-party library headers
  4. Project headers

- Group with a blank line between categories:

    #pragma once

    #include <string>
    #include <vector>

    #include <sqlite3.h>

    #include "core/application.h"
    #include "storage/application_repository.h"

---

### 4. Naming conventions

#### 4.1 Types

- **Classes / structs / enums / typedefs:** `PascalCase`
  - `Application`, `Statistics`, `SqliteDatabase`, `CommandType`
- **Namespaces:** lower snake_case
  - `namespace string_utils {}`, `namespace datetime {}`

#### 4.2 Functions

- **Free functions and member functions:** lower snake_case  
  Examples:
  - `today_iso()`
  - `fetch_applications()`
  - `compute_statistics()`
  - `insert()`
  - `find_by_id()`

#### 4.3 Variables and data members

- **Local variables & parameters:** lower snake_case

    auto next_id = 0;
    const std::string &database_path;

- **Data members:** lower snake_case, trailing underscore:

    int next_id_;
    SqliteDatabase database_;
    Config config_;

#### 4.4 Enums

- **Enum types:** `PascalCase`
- **Enum values:** `PascalCase` (or `SCREAMING_SNAKE_CASE` if more suitable)

    enum class CommandType
    {
        None,
        List,
        Add,
        ImportCsv,
        ImportRemoteCsv,
        ImportImap
    };

---

### 5. Documentation (Doxygen)

We use **Doxygen-style comments** consistently.

#### 5.1 General rules

- All API documentation is in **English**.
- For **classes, structs, interfaces and public functions**:
  - Use `/** ... */` block comments.
- For **fields / data members**:
  - Use `///` single-line comments.

#### 5.2 Examples

**Class / struct:**

    /**
     * @brief Aggregated statistics for job applications.
     */
    struct Statistics
    {
        /// Number of applications grouped by status (e.g. "applied" -> 10).
        std::map<std::string, int> count_by_status;
    };

**Interface:**

    /**
     * @brief Abstract repository interface for storing and retrieving job applications.
     *
     * This interface allows different storage backends (SQLite, PostgreSQL, etc.)
     * to be plugged in behind the same API.
     */
    class IApplicationRepository
    {
    public:
        /**
         * @brief Insert a new application and return the persisted entity.
         *
         * @param application Application to insert. Its id field may be 0.
         * @return Application with an assigned id and any storage-managed fields.
         */
        virtual Application insert(const Application &application) = 0;

        // ...
    };

**Utility functions in namespaces:**

    namespace datetime
    {
        /**
         * @brief Get today's date in ISO format (YYYY-MM-DD) using local time.
         *
         * @return A string containing the current local date in ISO format.
         */
        std::string today_iso();
    }

---

### 6. Error handling and return types

- Use `std::optional<T>` when a value can *legitimately be absent*.

    std::optional<Application> find_by_id(int id);

- Use `bool` to signal simple success/failure when no extra error info is needed.

    bool remove(int id);

- Throw `std::runtime_error` (or a more specific exception) when an operation **cannot proceed**:
  - Database cannot be opened.
  - SQL schema creation fails.

- For higher-level import logic, prefer returning an empty container rather than throwing on “soft” errors, and log the problem at the call site.

---

### 7. Testing conventions (Catch2)

- Test framework: **Catch2 v3**.
- Test files live under `tests/`, mirroring the module they test.
- File naming: `test_<component>.cpp`, e.g.:
  - `tests/util/test_string_utils.cpp`
  - `tests/import/test_remote_csv_import_source.cpp`

#### 7.1 `TEST_CASE` style

- Use descriptive names in English.

    #include <catch2/catch_test_macros.hpp>

    #include "util/string_utils.h"

    TEST_CASE("trim_removes_leading_and_trailing_whitespace")
    {
        const std::string input = "  hello  ";

        const auto output = string_utils::trim(input);

        REQUIRE(output == "hello");
    }

- Brace style and indentation follow the same rules as production code.

#### 7.2 Fakes and test doubles

- Test-only fakes live under `tests/<module>/`, e.g.:
  - `tests/storage/fake_application_repository.h`
  - `tests/import/fake_import_source.h`
- They are small in-memory implementations of the corresponding interface and should be clearly documented as “used only in tests”.

---

### 8. CMake conventions

- Each logical module has its own `CMakeLists.txt`:
  - `src/core/CMakeLists.txt` → `jobtracker_core`
  - `src/storage/CMakeLists.txt` → `jobtracker_storage_sqlite`
  - `src/import/CMakeLists.txt` → `jobtracker_import`
  - `src/cli/CMakeLists.txt` → `jobtracker_cli_lib` + `jobtracker_cli`
- The top-level `src/CMakeLists.txt` only calls `add_subdirectory(...)` for modules.
- Tests live in a single `jobtracker_tests` executable that links against:
  - `jobtracker_core`
  - `jobtracker_storage_sqlite`
  - `jobtracker_import`
  - `jobtracker_cli_lib`

---

### 9. Git and commits

- Make **small, focused commits** (one logical change per commit).
- Use clear, English commit messages:
  - Prefix: `feat:`, `fix:`, `chore:`, `docs:`, `refactor:`, `test:`, etc.

**Example:**

    feat(import): add remote CSV import source

    - Introduce RemoteCsvImportSource to fetch applications from HTTP-accessible CSV
    - Add IHttpClient abstraction and fake implementation for unit tests
    - Cover success and HTTP error cases in test_remote_csv_import_source.cpp

---

If you are unsure about a specific style decision, prefer **consistency with existing code** in the same module.  
When in doubt, keep it simple and readable.
