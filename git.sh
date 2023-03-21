array=($(find ./ -maxdepth 6  -type f -size +20M))

for i in "${array[@]}"
do
    git lfs track $i
done
# eval "$array='$(find ./test/model/ -maxdepth 6  -type f -size +20M)'"