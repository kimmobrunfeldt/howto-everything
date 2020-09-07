# Git tricks

### Move changes from one commit to another

There's a script that may help you: https://github.com/MikhailGolubtsov/git-mv-changes

I was scared to run that script so ended up doing:

* `git rebase -i master`
* Change the wanted commit to `edit`. Save text buffer and exit.
* `git reset HEAD~1` to reset the commit because with the recommended --amend command it's tough to remove edits from a file.
* `git checkout tests/myaccidentalfilechangesinwrongcommit.js` (to remove the changes)
* `git add . -A`
* `git commit -m "<copy paste the git commit message from github where we still have the old branch>"`.
* `git rebase --continue`
* Resolve possible conflicts. Now we have re-assembled the commit with changes removed.
* Go to github, and paste the contents of *tests/myaccidentalfilechangesinwrongcommit.js* file to local disk
* `git add tests/myaccidentalfilechangesinwrongcommit.js`
* Run `git log` to find out the hash of the commit where the file changes actually belonged to. Copy the hash.
* `git commit --fixup <hash>`
* `git rebase -i master --autosquash`. Then just save buffer and exit.
* `git push -f`
