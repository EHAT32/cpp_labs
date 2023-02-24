VAR=$(date '+%d.%m.%Y.%H.%M.%S') #date in given format
git checkout prd #switch to prd
git merge --commit stg #merge with stg
git tag "$VAR" #tag the date
git push origin prd
git push origin "$VAR"