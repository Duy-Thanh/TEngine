#! /bin/bash
git add .

COMMIT_MESSAGE=""
read -p "Commit message: " COMMIT_MESSAGE

git status
git commit -m $COMMIT_MESSAGE

# Write log and patch
echo >> master.patch
echo >> master.patch
git diff HEAD >> master.patch
git log --sparse --full-history >> history.patch
