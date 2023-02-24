#!/bin/bash
VAR=$(date '+%d.%m.%Y.%H.%M.%S') #date in format day-month-year-hour0-minute-second
git checkout stg #switch to stg
git merge --commit dev #merge and commit
git tag "$VAR"
git push origin stg
git push origin "$VAR"