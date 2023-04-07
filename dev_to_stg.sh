VAR=$(date '+%d.%m.%Y.%H.%M.%S') #date in given format
git checkout stg #switch to stg
git merge dev #merge dev into stg
git tag "$VAR" #tag the date
git push origin stg
git push origin "$VAR"
git checkout dev #switch to dev
git commit -m "dev merged into stg"