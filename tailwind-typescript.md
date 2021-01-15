# Typing Tailwind and classnames

This typing allows a strictly typed wrapper for [classnames](https://www.npmjs.com/package/classnames) module .

If you want a solution to automatically generate Tailwind class types from your config, see: https://github.com/esamattis/postcss-ts-classnames. Compile 
time might be slow so beware.

Credits to:

* https://twitter.com/virtualkirill/status/1332340074780901379
* https://dev.to/virtualkirill/make-your-css-safer-by-type-checking-tailwind-css-classes-2l14
* https://stackoverflow.com/questions/65737948/how-to-type-check-if-object-keys-conform-a-conditional-recursive-template-type/65738172#65738172


## Typings

```typescript
// Credits to https://dev.to/virtualkirill/make-your-css-safer-by-type-checking-tailwind-css-classes-2l14
// and https://stackoverflow.com/questions/65737948/how-to-type-check-if-object-keys-conform-a-conditional-recursive-template-type/65738172#65738172
type Colors = "red" | "purple" | "blue" | "green";
type Luminance = 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900;
type BgColor = `bg-${Colors}-${Luminance}`;
type Layout = "block" | "w-1" | "h-1";
type TailwindClass = BgColor | Layout;

type EOL = "";
type EatWhitespace<S> = S extends ` ${infer Rest}` ? EatWhitespace<Rest> : S;

// Utility type to provide nicer error messages
type Err<Cls extends string = EOL> = Cls extends EOL ? "Error: Empty class" : `Error: ${Cls} is not a valid Tailwind class`;
type Result<R extends string = EOL> =
  R extends `${infer _}Error: ${infer Err}`
    ? Err extends `${infer Top} Error: ${infer _}`
      ? `Error: ${Top}`
      : `Error: ${Err}`
    : R;

// Helper type to validate that a string contains one or multiple valid
// Tailwind classes separated by whitespace
type ValidTailwindClassSeparatedByWhitespace<S> =
  EatWhitespace<S> extends `${infer Class} ${infer Rest}`
  ? Rest extends EOL
    ? Class extends EOL
      ? S
      : Class extends TailwindClass 
        ? Class
        : Err<Class>
    : Result<`${ValidTailwindClassSeparatedByWhitespace<Class>} ${ValidTailwindClassSeparatedByWhitespace<Rest>}`>
  : EatWhitespace<S> extends `${infer Class}`
    ? Class extends TailwindClass
      ? Class
      : Err<Class>
    : Err;

type Checked = ValidTailwindClassSeparatedByWhitespace<"bg-red-100">;


type ClassNames<R> = { [K in keyof R]: K extends ValidTailwindClassSeparatedByWhitespace<K> ? R[K] : ValidTailwindClassSeparatedByWhitespace<K> };

function classNamesWrapper<S1, S2>(
  classesOrModifiers1: S1 extends string ? ValidTailwindClassSeparatedByWhitespace<S1> : ClassNames<S1>,
  classesOrModifiers2?: S2 extends string ? ValidTailwindClassSeparatedByWhitespace<S2> : ClassNames<S2>,
): string {
  // All arguments would be passed to npmjs.com/package/classnames
  // For the example, just return empty string.
  return '';
}

// Good
classNamesWrapper('block', { 
  "bg-red-100": true, 
  "block w-1": true 
});
classNamesWrapper('block')
classNamesWrapper({ 
  "bg-red-100": true, 
  "block w-1": true 
});
// This should be ok by typing, but not really a use case
classNamesWrapper({ 
  "bg-red-100": true, 
  "block w-1": true 
}, { 
  "bg-red-100": true, 
  "block w-1": true 
});



// Bad
classNamesWrapper('block', { 
  "bad-class": false,  // only this key should be invalid
  "block h-1": true
});
classNamesWrapper('blaock', { 
  "bad-class": false,  // only this key should be invalid
  "block h-1": true
});
classNamesWrapper('blkkoack')
classNamesWrapper({ 
  "bad-0": true, 
  "block w-1": true 
});
classNamesWrapper({ 
  "bad-100": true, 
  "block w-1": true 
}, { 
  "bg-red-100": true, 
  "bad w-1": true 
});
```
