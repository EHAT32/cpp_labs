git switch dev
git merge --commit main 
git push
git switch stg
git merge --commit dev 
git push
git switch prd
git merge --commit stg 
git push
git switch main
@pause
