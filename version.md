# Yutiyeff

## Current version
### 0.2.0
#### ADD
- ability to change/set a single code point manually.
- ability to remove a given range
#### CHANGE
- code style: removed "virtual" from beginning of overriding methods (they already have override).
#### FIX
- operator [] not converting to Utf32String first before indexing code point, leading to errors and possible crashes.
- more explicit operators == and != to compare like types without ambiguity.

## Previous versions
### 0.1.0
- usable Utf8String, Utf16String and Utf32String.
