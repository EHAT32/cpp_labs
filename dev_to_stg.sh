VAR=$(date '+%d.%m.%Y.%H.%M.%S') #date in given format
git checkout stg #switch to stg
git merge --commit dev #merge with dev
git tag "$VAR" #tag the date
git push origin stg
git push origin "$VAR"