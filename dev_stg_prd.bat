git switch stg
git merge dev -m "stg and dev" 
git push
git switch prd
git merge stg -m "prd and stg" 
git push
git switch dev
@pause