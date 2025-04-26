---
Generator: Umba Brief Scanner
---

# _libs/encoding/compact_enc_det

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1982]`
  initial pointer is to FRONT of string, not back

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2062]`
  Encoding probabilites are all zero at this point

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2519]`
  this requires at least one lower AND one upper AND one digit to pass

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5183]`
  destate and scoped_debug_data have exactly the same scope All other
  FLAGS_enc_detect_detail tests use destate.debug_data != NULL

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5246]`
  Encoding probabilites are all zero at this point

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5255]`
  probabilities up to this point are subject to derating for small numbers of
  bigrams. Probability changes after this point are not derated.

- `[_libs/encoding/compact_enc_det/compact_enc_det_unittest.cc:1986]`
  Unicode handled differently because of embedded NUL

- `[_libs/encoding/compact_enc_det/compact_enc_det_unittest.cc:4745]`
  strlen will not work for UTF-16 and UTF-32 strings with embedded NUL



# _libs/encoding/util

- `[_libs/encoding/util/basictypes.h:49]`
  unsigned types are DANGEROUS in loops and other arithmetical places.  Use the
  signed types unless your variable represents a bit pattern (eg a hash value) or
  you really need the extra bit.  Do NOT use 'unsigned' to express "this value
  should always be positive"; use assertions for this.

- `[_libs/encoding/util/basictypes.h:115]`
  that the function doesn't need an implementation, as we only use its type.



# _libs/encoding/util/encodings

- `[_libs/encoding/util/encodings/encodings.cc:48]`
  that the preferred MIME name may differ slightly from the official IANA name:
  i.e. ISO-8859-1 vs. ISO_8859-1:1987

- `[_libs/encoding/util/encodings/encodings.cc:216]`
  that MimeEncodingName() returns "Shift_JIS", since KDDI uses "Shift_JIS" in
  HTTP headers and email messages.

- `[_libs/encoding/util/encodings/encodings.cc:459]`
  that despite an RFC to the contrary, ARABIC_ENCODING (ISO-8859-6) is NOT
  visual.

- `[_libs/encoding/util/encodings/encodings.cc:576]`
  that MimeEncodingName() can return the same mime encoding name for different
  encoding enums like JAPANESE_SHIFT_JIS and KDDI_SHIFT_JIS.  In that case, the
  encoding enum first seen will be the value for the encoding name in the map.

- `[_libs/encoding/util/encodings/encodings.h:39]`
  The Encoding enum must always start at 0. This assumption has been made and
  used.

- `[_libs/encoding/util/encodings/encodings.h:146]`
  that the name of this function is somewhat misleading. There is nothing "right
  to left" about these encodings. They merely contain code points for characters
  in RTL languages such as Hebrew and Arabic. But this is also true for UTF-8.



# _libs/encoding/util/languages

- `[_libs/encoding/util/languages/languages.h:122]`
  Technically we're talking about scripts, not languages. There are languages
  that can be written in more than one script. Examples: - Kurdish and Azeri
  ('AZERBAIJANI') can be written left-to-right in Latin or Cyrillic script, and
  right-to-left in Arabic script. - Sindhi and Punjabi are written in different
  scripts, depending on region and dialect. - Turkmen used an Arabic script
  historically, but not any more. - Pashto and Uyghur can use Arabic script, but
  use a Roman script on the Internet. - Kashmiri and Urdu are written either with
  Arabic or Devanagari script.
  
  This function only returns true for languages that are always, unequivocally
  written in right-to-left script.

- `[_libs/encoding/util/languages/languages.h:150]`
  See important notes under IsRightToLeftLanguage(...).
  
  This function returns true for languages that *may* appear on the web in a
  right-to-left script, even if they may also appear in a left-to-right script.
  
  This function should typically be used in cases where doing some work on
  left-to-right text would be OK (usually a no-op), and this function is used
  just to cut down on unnecessary work on regular, LTR text.

- `[_libs/encoding/util/languages/languages.h:250]`
  See the note below about the codes for Chinese languages.

- `[_libs/encoding/util/languages/languages.h:265]`
  CHINESE LANGUAGE CODES
  
  There are three functions that return codes for Chinese languages.
  LanguageCode(lang) and LanguageCodeWithDialects(lang) are defined here.
  LanguageCode(lang, encoding) is defined in i18n/encodings.lang_enc.h. The
  following list shows the different results.
  
  LanguageCode(CHINESE) returns "zh" LanguageCode(CHINESE_T) returns "zh-TW".
  
  LanguageCodeWithDialects(CHINESE) returns "zh-CN".
  LanguageCodeWithDialects(CHINESE_T) returns "zh-TW".
  
  LanguageCode(CHINESE_T, <any encoding>) returns "zh-TW". LanguageCode(CHINESE,
  CHINESE_BIG5) returns "zh-TW". LanguageCode(CHINESE, <any other encoding>)
  returns "zh-CN".
  
  --------------------------------------------

- `[_libs/encoding/util/languages/languages.pb.h:187]`
  If you add a language, you will break a unittest. See the note at the top of
  this enum.



# _libs/marty_decimal

- `[_libs/marty_decimal/marty_bcd_decimal_impl.h:140]`
  Вообще не паримся по поводу несимметричности множества целых относительно нуля



# _libs/marty_decimal/tests/src

- `[_libs/marty_decimal/tests/src/benchmark_tests.cpp:852]`
  cpp_dec_float: 1) гарантирует 50 значащих десятичных цифр всего, а не после
  десятичной точки 2) Немотря на название, не является десятичным числом -
  косвенным подтверждением этого является то, что на рекурентном тесте Мюллера
  его расколбашивает точно так же, как и встроенный в язык double. dec в названии
  говорит только о том, что данный тип хранит не менее, чем заданное число
  десятичных знаков.



# _libs/umba

- `[_libs/umba/exception.h:255]`
  - required to be using namespace ::umba::omanip or using namespace std::iomanip
  declared before using this macro

- `[_libs/umba/filecache.h:16]`
  Not safe for threading



# _libs/umba/internal

- `[_libs/umba/internal/winerror_org.h:22986]`
  This assumes that WIN32 errors fall in the range -32k to 32k.
  
  Define bits here so macros are guaranteed to work

- `[_libs/umba/internal/winerror_org.h:31141]`
  that additional FACILITY_SSPI errors are in issperr.h
  
  ****************** FACILITY_CERT ******************
  
  MessageId: TRUST_E_PROVIDER_UNKNOWN
  
  MessageText:
  
  Unknown trust provider.

