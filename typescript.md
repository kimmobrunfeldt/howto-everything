# Typescript 

Links and examples for TS.


## Template literals

* https://dev.to/virtualkirill/make-your-css-safer-by-type-checking-tailwind-css-classes-2l14
* https://stackoverflow.com/questions/65737948/how-to-type-check-if-object-keys-conform-a-conditional-recursive-template-type/65738172#65738172
* Other helpful resources around the subject:
* https://devblogs.microsoft.com/typescript/announcing-typescript-4-1-beta/#template-literal-types
* https://github.com/microsoft/TypeScript/pull/40336
* https://github.com/ghoullier/awesome-template-literal-types
* https://github.com/g-plane/type-gymnastics
* https://github.com/0kku/destiny/blob/a3d5182658599bc117db3b4657fed71dfedf8b44/src/parsing/XmlTypeParser.ts
* https://github.com/g-plane/type-gymnastics/blob/43909eb35dafd3bb927da41d888350b6d9167d88/src/spec-compliant-html-parser/index.ts
* https://github.com/gmono/ts-metacode/blob/dd3c34488195aefe4ca688c169aba31873cb0d82/src/string.ts

## Validate characters

Suffers from recursion limit with long strings

```typescript
// Utility type to provide nicer error messages
type Err<Message extends string> = `Error: ${Message}`;


type LowerAlpha = 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z';
type UpperAlpha = Uppercase<LowerAlpha>;
type Digit = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9';
type SpecialChars = ':' | '_' | '-';

// https://www.w3.org/TR/CSS21/grammar.html#scanner
type CssClassChars = LowerAlpha | UpperAlpha | Digit | SpecialChars;

// Validates given `UnvalidatedInput` input string
// It recursively iterates through each character in the string,
// and appends valid characters into the second type parameter `Validated`
// NOTE: Do not pass anything else than the first type parameter. Other type
//       parameters are for internal tracking during recursive loop
//
// See https://github.com/microsoft/TypeScript/pull/40336 for template literal
// examples. The mechanism is also explained in the PR description:
//
//  "Type inference supports inferring from a string literal type to a
//   template literal type. For inference to succeed the starting and ending
//   literal character spans (if any) of the target must exactly match the
//   starting and ending spans of the source. Inference proceeds by matching
//   each placeholder to a substring in the source from left to right: A
//   placeholder followed by a literal character span is matched by inferring
//   zero or more characters from the source until the first occurrence of that
//   literal character span in the source. A placeholder immediately followed
//   by another placeholder is matched by inferring a single character from
//   the source.""
type ValidCssClassName<UnvalidatedInput extends string, Validated extends string = '', RecursionStarted extends boolean = false> =
  // Have we reached to the end of the input string ?
  UnvalidatedInput extends EndOfInput
    // Yes, and has the recursive loop been started?
    ? RecursionStarted extends true
      // Also yes, so we are at the end of input and all characters were valid
      ? Validated
      // No, it means empty string was passed
      : Err<"Empty input">
    // No, use template literal inference to get first char, and the rest of the string
    : UnvalidatedInput extends `${infer Head}${infer Rest}`
      // Is the first char valid?
      ? Head extends CssClassChars
        // Yes, continue to next character
        ? ValidCssClassName<Rest, `${Validated}${Head}`, true>
        // No, "throw" an error
        : Err<`'${Head}' is not valid CSS class name character`>
      // This shouldn't happen since UnvalidatedInput is restricted with
      // `extends string` type narrowing.
      // For example ValidCssClassName<null> would be a `never` type if it didn't
      // already fail to "Type 'null' does not satisfy the constraint 'string'"
      : never
```
