```typescript
/**
 * Creates a Proxy which throws an explicit error when
 * an unintended property is accessed.
 *
 * The Proxy is useful for a test scenario where we create a mock object that
 * only contains partial information that the real object would contain. In case
 * the code path happens to access a property that was not intended, we get an
 * error instead of the code continuing to run an unintended code path with
 * `undefined` or `'MOCK_VALUE_DO_NOT_USE'` value.
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy
 */
export function createStrictProxy(obj: any): any {
  return new Proxy(obj, {
    get: (target, prop, receiver) => {
      if (!_.has(obj, prop)) {
        throw new Error(
          `Unexpected property of strict mock object was accessed: '${String(
            prop
          )}'. This error happens when an unintended code path was accessed for example in a test mocking scenario.`
        )
      }

      return Reflect.get(target, prop, receiver)
    }
  })
}
``` 
