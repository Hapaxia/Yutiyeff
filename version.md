# Yutiyeff

## Current version

### 0.5.0
- #### ADD
  - ability to construct from a char (8-bit, 16-bit or 32-bit).
  - operator + can now accept const char on left-hand-side.
  - "empty" method to identify if the string is empty (similar to STL's containers).
  - "size" method that returns the length (similar to STL's containers).
- #### CHANGE
  - slight optimisation of operator += (adds sequences directly).
  - code style: moved newly-added external operators from .hpp file to .inl file (with the other external operators).
- #### FIX
  - operator + now doesn't modify the class and is const.
  - moved external operators outside of the yutiyeff namespace (and into global scope).

---

## Previous versions

## 0.4.0
- #### CHANGE
  - swapped order or parameters in "insert" to match closer to STL's insert. this also removes default parameter for "offset".
- #### FIX
  - made external operators inline.

## 0.3.0
- #### ADD
  - ability to insert a single code point; previously could only insert a string.
- #### CHANGE
  - removed "remove" as "erase" already existed, doing the same task.
  - altered what "erase" does to match what "remove" did. i.e. works with code points rather than values in the sequence.
  - code style: moved "erase" to join the other modification methods and places "find" at the top with the other const method.

## 0.2.0
- #### ADD
  - ability to change/set a single code point manually.
  - ability to remove a given range.
- #### CHANGE
  - code style: removed "virtual" from beginning of overriding methods (they already have override).
- #### FIX
  - operator [] not converting to Utf32String first before indexing code point, leading to errors and possible crashes.
  - more explicit operators == and != to compare like types without ambiguity.

## 0.1.0
- #### ADD
  - usable Utf8String, Utf16String and Utf32String.
