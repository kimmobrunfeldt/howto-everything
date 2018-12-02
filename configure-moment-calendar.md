# Configure moment calendar

https://momentjs.com/docs/#/customization/calendar/

**Beware** that `moment.updateLocal` is globally mutating the moment behavior.

```js
// Update calendar locale. Thought behind this format is to be precise, but still have the
// convenience (last week etc.). Also it uses the 24h clock which is more common in Finland
//
// Nov 22nd 2018 at 14:59
// Nov 26th 2018 at 14:59 (Last week)
// Nov 29th 2018 at 14:59 (Last week)
// Dec 1st 2018 at 14:59 (Yesterday)
// Dec 2nd 2018 at 14:59 (Today)
// Dec 3rd 2018 at 14:59 (Tomorrow)
// Dec 5th 2018 at 14:59 (Next week)
// Dec 12th 2018 at 14:59
moment.updateLocale('en', {
  calendar: {
    lastDay: 'lll [(Yesterday)]',
    sameDay: 'lll [(Today)]',
    nextDay: 'lll [(Tomorrow)]',
    lastWeek: 'lll [(Last week)]',
    nextWeek: 'lll [(Next week)]',
    sameElse: 'lll',
  },
  // moment docs:
  // 'You can eliminate the lowercase l tokens and they will be created automatically by
  // replacing long tokens with the short token variants.'
  longDateFormat: {
    LT: 'HH:mm',
    LLL: 'MMMM Do YYYY [at] LT',
  },
});
```
