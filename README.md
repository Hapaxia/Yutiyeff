# Yutiyeff
by Hapaxia (https://github.com/Hapaxia)

## Description
Yutiyeff is a group of C++ string types that encode internally using different UTF versions.
Each Yutiyeff string type can be converted to any other Yutiyeff string type automatically.

## Types
Yutiyeff string types:
- Utf8String
> stored internally using UTF-8 encoding with a string of 8-bit values.
- Utf16String
> stored internally using UTF-16 encoding with a string of 16-bit values.
- Utf32String
> stored internally using UTF-32 encoding with a string of 32-bit values.

The different types have their own advantages and disadvantages.
- UTF-8 uses less storage space for a string of lower values so a basic ASCII-encoded string is the same size as a standard string.
However, it can be slightly slower or more complex to manipulate characters.
- UTF-16 uses more storage space but can encompass all of the code points considered most common (e.g. only quite 'special' code points are outside of the 16-bit range).
It can be simpler to use than UTF-8 as manipulation is mostly blocks of 16-bits.
However, this is not cover those characters outside of the range and uses multiple 16-bit characters to represent code points outside of the range.
- UTF-32 is the simplest version as all code points can be represented by a single 32-bit value so they can (usually) be treated and manipulated as individual values.
However, each code points is _always_ 32-bit so the size of the storage will always be the largest possible.
A basic ASCII-encoded string would require 4 times the storage space as a standard string (or UTF-8 string)

## Manipulation
Since all code points can be represented by a single 32-bit value, string manipulation is quite simple using UTF-32 but more complex for UTF-16 and even moreso for UTF-8.

Yutiyeff does do some direct processing on UTF-8 and UTF-16 but - in some cases - converts to UTF-32 before processing and then back again after processing.

Some more direct processing (for optimisation only) for UTF-8 and UTF-16 may be added in the future but the usage should stay the same.

## C++
Yutiyeff requires a C++ version of C++11 or later.  
It is also C++20-ready.
> Note: Yutiyeff determines whether the compiler is using C++20 using **__cplusplus**.

> If you are using Visual Studio, **__cplusplus** is not automatically set to the correct value.  
> If you are using C++20 then you can fix this by adding **`/std:c++20 /Zc:__cplusplus`** to the compiler's additional options  
(**Project Properties**->**Configuration Properties**->**C/C++**->**All Options**->**Additional Options**)
