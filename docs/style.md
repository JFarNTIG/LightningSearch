# LightningSearch Code Style Guide  
*Version 1.0 – June 2025*

> **Purpose**  
> This document outlines a consistent, modern, and readable coding style for all contributors.
>   Target language standard: **C++17**

---

## 1  Repository Layout
| Item | Convention | Example |
|------|------------|---------|
| Source files | `*.cpp` | `CMainWindow.cpp` |
| Header files | `*.hpp` | `CMainWindow.hpp` |
| Public headers | live in `<project>/include/` | `program/include/ui/IFilterWidget.hpp` |
| Tests | live in `tests/` | `tests/src/test_CStreamSearcher.cpp` |

* One class ≈ one header/implementation pair.
* Keep **main.cpp** minimal - delegate work to classes and library code.

---

## 2  Formatting Rules
* **Indentation:** 4 spaces, no tabs.  
* **Line length:** Avoid lines which are too long. As a general rule, lines should not be longer than your IDE screen width.
* **Whitespace:** No spaces between keywords such as `if` and accompanying parentheses.
* **Braces:** *Stroustrup/Allman hybrid*  

    ```cpp
    if(condition) {
        doSomething();
    } else {
        handleElse();
    }
    ```
* **Types and qualifiers:** type, then qualifiers, then ref/pointer
    ```cpp
    bool bufferedMatch(wchar_t const *text,
                        size_t const size,
                        size_t const streamPos) const;
    ```
* **Namespace indentation:** increase one level; never indent `using` directives.
* **Blank lines:** one between logical blocks; none after opening brace or before closing brace.

---

## 3  Naming Conventions

| Entity             | Style              | Example                        |
| ------------------ | ------------------ | ------------------------------ |
| Namespace          | `lower_snake_case` | `nti::io`                      |
| Class / Struct     | `UpperCamelCase`   | `CMainWindow`                  |
| Function           | `lowerCamelCase`   | `createActions()`              |
| Variable           | `lowerCamelCase`   | `fileSize`                     |
| Member Variable    | `m_lowerCamelCase` | `m_matchText`                  |
| Enum type          | `UpperCamelCase`   | `ErrorCode`                    |
| Macro              | `ALL_CAPS_SNAKE`   | `MAX_BUFFER_SIZE`              |
| Template parameter | `UpperCamelCase`   | `template<typename ValueType>` |

Class names should be prefixed according to whether the class is an interface (`IFilter`) or a concrete class (`CMainWindow`). 

---

## 4  Language Guidelines

* Always use `const` for constants.
* Prefer references for function parameters where possible, to avoid unnecessary copies and encourage memory efficiency.
* Mark single-arg constructors **explicit**.
* Use `noexcept` on functions that should not throw exceptions.
* Use scoped enums (`enum class`) instead of unscoped enums.

---

## 5  Includes & Forward Declaration

1. **Order:**
   1. Corresponding header (`"<CMainWindow.hpp>"`).
   2. Project headers (`<search/...>`).
   3. Third-party headers.
   4. Standard library headers (`<vector>`).
2. Use angle-brackets (`<...>`) for project headers.
3. Never include unused headers.

---

## 6  Error Handling

* Prefer **exceptions** for unrecoverable errors.
* Don't leak exceptions across ABI binaries - translate to error codes instead.
* Catch by **const reference** ; re-throw with `throw`.

---

## 7  Concurrency

* Protect shared state with **`std::mutex`** or atomic primitives.
* Avoid global variables.

---

## 8  Documentation & Comments

* Inline comments should clarify any complex logic.
* Mark TODOs with `// TODO: ...`.
* For public APIs, use **Doxygen**-style block comments. Function usage, parameters, return value and any important considerations should be clearly documented.
* If a function deals with pointers, it should be clearly documented what code is considered to have ownership of the pointer and what code is eventually responsible for freeing memory.
  Examples:
  ```cpp
  /**
    * @brief Create a new filter object based on the widget's input fields.
    *
    * NOTE: The caller takes ownership of the returned filter object and is responsible for deleting it.
    */
   virtual IFilter *createFilter() = 0;
  ```
  ```cpp
  /**
     * @brief Compare a chunk from an input stream against a chunk
     * in the match string.
     * @param bufferPos index of the start of the chunk within the stream
     * @param text pointer to start of the chunk from the stream
     * @param size size of the chunk
     * @return true if the chunk matches, false otherwise
     */
    bool bufferedMatch(wchar_t const *text, size_t const size, size_t const streamPos) const;
  ```

---

## 9  Unit Testing


* Framework: **gtest**.
* Unit tests should be thorough. Always include tests to cover edge cases and error conditions.
* Every feature update should have accompanying unit tests, wherever unit tests are possible.
* Every bug fix should have an accompanying unit test.
* Tests must be deterministic.
* Tests should be as efficient as possible; aim to run in <100ms.

---

## 10  Git Workflow

1. **Branch names:** `feature/short-desc` or `bugfix/short-desc`.
2. **Commit messages:** Short and informative. Always write in present or imperative tense.

   ```
   Short description (<= 72 characters)
   ```

   Examples:
   ```
   Fix inconsistent style in IFilter definition
   ```
   ```
   Add doc comments for CSearchEngine methods
   ```


3. **Pull Requests:**
   * PR descriptions should be informative and clearly indicate the goals of the PR and testing status.
   * Title written in present or imperative tense.
   * *Types*: `feat`, `fix`, `docs`, `refactor`, `test`, `ci`, `build`, `perf`.
   * At least one approving review required; reviewers use GitHub Suggested Changes.
   * When creating a pull request, always use the PR template:

   ```markdown
   Title: [type] Short description

   Description:
   Informative description of what the PR is supposed to accomplish.
   Additionally, fill out the PR checklist:

   - [ ] My code follows the style guide (`docs/style.md`)
   - [ ] Unit tests were added
   - [ ] Unit tests will be added later after some review
   - [ ] Unit tests weren't necessary
   ```

   Example:
   ```
   Title: [feat] New class for stream regex search

   Description:
   This PR adds a class CStreamRegexSearcher. It implements IStreamSearcher
   (just like CStreamSearcher) and overrides `searchText`.
   The regex searcher supports searching in a stream by regular expression.
   Unlike non-regex search like in CStreamSearcher, it is not
   as straightforward to do buffered search with regex, so my implementation simply
   reads the entire stream contents into memory and applies `std::wregex`.

   - [X] My code follows the style guide (`docs/style.md`)
   - [ ] Unit tests were added
   - [X] Unit tests will be added later after some review
   - [ ] Unit tests weren't necessary
   ```

---

## 11  Deprecation Policy

* Mark outdated APIs with `[[deprecated("Use FooV2")]]`.
* Provide adapters for **one minor release** before removal.

---

## 12  License Header

Add to every source file:

```cpp
// SPDX-License-Identifier: GPL-2.0
```

---

## 13  Acknowledgements

Inspiration was taken from *LLVM*, *Google C++ Style Guide*, and *C++ Core Guidelines*.
