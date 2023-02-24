VAR=$(date '+%d.%m.%Y.%H.%M.%S') #date in given format
git checkout prd #switch to prd
git merge stg #merge stg into prd
git tag "$VAR" #tag the date
git push origin prd
git push origin "$VAR"
git checkout dev #switch to dev
git commit -m "stg merged into prd"