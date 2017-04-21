#!/bin/bash

# Adapted from
# https://mike42.me/blog/2014-04-including-git-commit-history-in-a-latex-document

# Find remote URL for hashes (designed for GitHub-hosted projects)
origin=`git config remote.origin.url`
base=`dirname "$origin"`/`basename "$origin" .git`

# Output LaTeX table in reverse chronological order
echo "\\begin{tabular}{p{1.5cm} p{4cm} p{3cm} p{8cm}}\\textbf{Commit} & \\textbf{Date} & \\textbf{Author} & \\textbf{Description}\\\\\\hline"
git log --pretty=format:"\\href{$base/commit/%H}{%h} & \\detokenize{%cd} & \\detokenize{%an} & \\detokenize{%s}\\\\\\hline" --date=format:'%d %B %Y %I:%M %p'
echo "\\end{tabular}"
