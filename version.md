# Yutiyeff

## Current version
### 0.3.0
- #### ADD
  - ability to insert a single code point; previously could only insert a string.
- #### CHANGE
  - removed "remove" as "erase" already existed, doing the same task.
  - altered what "erase" does to match what "remove" did. i.e. works with code points rather than values in the sequence.
  - code style: moved "erase" to join the other modification methods and places "find" at the top with the other const method.

## Previous versions
### 0.2.0
- #### ADD
  - ability to change/set a single code point manually.
  - ability to remove a given range.
- #### CHANGE
  - code style: removed "virtual" from beginning of overriding methods (they already have override).
- #### FIX
  - operator [] not converting to Utf32String first before indexing code point, leading to errors and possible crashes.
  - more explicit operators == and != to compare like types without ambiguity.

### 0.1.0
- #### ADD
  - usable Utf8String, Utf16String and Utf32String.
