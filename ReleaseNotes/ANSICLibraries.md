# 3.3 Release Notes: ANSI C Libraries

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This release note mentions important items with regard to NEXTSTEP's ANSI C libraries.

---

## Notes Specific to Release 3.1

### Known Problems

| Reference | 34690 |
|-----------|-------|
| **Problem** | The `atof(3)` and `strtod(3)` functions produce different results for non-numbers on m68k and i386 architectures. |
| **Description** | On the m68k architecture, `atof()` and `strtod()` improperly interpret a leading 'i', 'n', or 's' as representing the floating-point values Infinity, NaN, and SNaN, respectively. On the i386 architecture these functions properly return 0.0 for the above cases, as well as for any other non-numeric string. In addition to producing different results, this may surprise users who've been used to typing values like "infinity" in text fields. |
| **Workaround** | To insure consistent results, your code should check that the first non-whitespace, non-sign character of a string is a digit or decimal separator before calling `atof()` or `strtod()`. |
| | If you want to accept string values such as "infinity", your code should explicitly check for them before calling `atof()` or `strtod()`. For example, it should check the return of a Control's or Cell's `stringValue` method rather than rely on the `floatValue` or `doubleValue` methods. |
