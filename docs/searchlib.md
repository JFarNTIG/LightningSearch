## Stream Searching

The core purpose of LightningUtil is to provide functionality for fast, memory-efficient searching.

The most basic task, searching in text, can be easily done with `CStreamSearcher`.

```cpp
// First, we create a stream from the text we want to search in.
std::wstringstream ss("Hello World!");

// parameters:
// - text to search for
// - case insensitive (true=ignore case, false=match case)
// - whole or partial match (true=match, false=search)
CStreamSearcher searcher("World", false, false);

// result should be: true
bool result = searcher.searchText(ss);
```

`CStreamSearcher` can search or match any Unicode stream. This means that you can open a file using `std::wifstream` and pass it to `CStreamSearcher`, and it will search the file contents:

```cpp
std::wifstream file("header.hpp");

CStreamSearcher searcher("#pragma once", false, false);
bool result = searcher.searchText(file);
```

`CStreamSearcher` performs buffered search or match. It won't read in the entire file into memory at once, but rather in chunks. This means that `CStreamSearcher` will work properly even for very large files.

By default, the chunk size is 1 megabyte. However, you can set a custom chunk size. This might be helpful when running on memory-constrained systems, or systems with large amounts of RAM:

```cpp
std::wifstream file("header.hpp");

// use a chunk size of 5000 bytes.
CStreamSearcher searcher("#pragma once", false, false, 5000);
bool result = searcher.searchText(file);
```