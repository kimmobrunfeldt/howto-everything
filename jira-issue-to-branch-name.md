# Bookmarklet to easily generate branch name from JIRA ticket

Thanks to [@jareware](https://github.com/jareware) the code, I made small modifications to the script.


## Installation


Paste the following code in [Bookmarkleter](https://chriszarate.github.io/bookmarkleter/) and follow instructions how to setup the bookmark.

```javascript
(() => {
  const key = (location.href.match(/selectedIssue=([A-Z]+-\d+)$/) || [])[1]
  if (!key) return alert('Error: URL does not look right')
  const h1 = document.querySelectorAll('div[role=dialog] h1')
  if (h1.length < 1) return alert('Error: DOM does not look right')
  const title = h1[0].innerText.toLowerCase()
    .replace(/[^a-z]+/g, '-')
    .replace(/(^-|-$)/g, '')
  const branchName = key.toLowerCase() + '-' + title;

  navigator.clipboard.writeText(branchName).then(() => {
    alert('Copied to clipboard: ' + branchName);
  }).catch((err) => {
    alert('Could not copy text: ' + err);
  });
})();
```
