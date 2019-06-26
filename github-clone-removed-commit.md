# Recover a removed commit on github

When using `git rebase --interactive`, `git push origin --force` and `git gc` it's possible to run into a situation where
you accidentally lose track of a give commit. Alternative solutions include

1. If you know the reference to the commit, simply check and create the branch using `git checkout -b name/of/branch <sha1>``
2. If you're not sure about the SHA-1 of the commit, use `git reflog` to check past commits to figure out which one is the correct one
and then repeat step 1.
3. If you have run `git gc` and removed local unreachable commits (or even worse, lost your laptop), but the commit exists on github, you cannot simply fetch the commit, as this functionality is disabled in normal cases:

```sh
⇒  git fetch origin 4e27a3f4f316482d1bfc629707ed250b119075c2
error: Server does not allow request for unadvertised object 4e27a3f4f316482d1bfc629707ed250b119075c2
```

This is due to github only allowing to access named objects (branches or tags), but prevents
accessing commits, which are not reachable through names. These commits are ephemeral (unless a
successor is a reachable root), and will eventually be cleaned out by garbage collection.

In order to be able to fetch those commits if all other methods have failed, you can use the
github API to make the commit reachable, and then clone it using the following steps:

0. Create an API-token with repository access on the UI, if you don't already have one: https://github.com/settings/tokens
1. Figure out the SHA-1 sum of the commit. You should be able to access it on the UI, even if it's not in the history: https://github.com/:ACCOUNT_NAME/:REPOSITORY_NAME/commit/:SHA1
2. Create a reference to the commit with the API, using your created token:
```sh
curl -v -X POST --user '<username>:<token>' https://api.github.com/repos/:ACCOUNT_NAME/:REPOSITORY_NAME/git/refs --data '{
  "ref": "refs/heads/branch-pointing-to-lost-commit",
  "sha": ":SHA1"
}'
```
3. Run `git fetch` to fetch all teh commits to the local repo. You should receive a reference to
`origin/branch-pointing-to-lost-commit`, which is now a normal branch you can work with.
4. (Optionally) Disable the created API-token

