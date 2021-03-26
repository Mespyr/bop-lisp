git add .
if [ "$1" == "" ]; then
    printf "Commit Message: "
    read msg
else
    msg="$1"
fi
git commit -m "$msg"
git push