---
Generator: Umba Brief Scanner
---

# _libs/encoding/util

- `[_libs/encoding/util/basictypes.h:281]`
  if Dest or Source is a non-POD type, the result of the memcpy is likely to
  surprise you.



# _libs/encoding/util/encodings

- `[_libs/encoding/util/encodings/encodings.h:115]`
  This function does not currently return true for all encodings that are
  supersets of Ascii 7-bit.

- `[_libs/encoding/util/encodings/encodings.h:122]`
  This function does not currently return true for all encodings that are 8-bit
  encodings.



# _libs/encoding/util/languages

- `[_libs/encoding/util/languages/languages.h:106]`
  This function provides only a simple test on the values of the two Language
  arguments. It returns false if either language is invalid. It returns true if
  the language arguments are equal, or if they are both Chinese languages, both
  Norwegian languages, or both Portuguese languages, as defined by
  IsChineseLanguage, IsNorwegianLanguage, and IsPortugueseLanguage. Otherwise it
  returns false.



# _libs/umba/warnings

- `[_libs/umba/warnings/disable_unsafe_conversion.h:1]`
  C4191: 'type cast': unsafe conversion from 'FARPROC' to '...'

