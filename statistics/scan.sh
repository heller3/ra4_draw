#! /bin/bash

shopt -s nullglob

if [ $# -lt 1 ]
then
    echo "Syntax: ./scan.sh <name_of_variation_folder> <0 to skip datacard to workspace conversion> <specify number of cores (default 24)>."
    echo "Make sure to use environment with combine installation (i.e. source setup.sh)"
    exit 1
fi

lim_dir=$1
channel_mask=$2
num_parallels=24
make_workspaces=1

##Check if workspace conversion should be skipped
if [ $# -gt 1 ]
then
    if [ $2 = 0 ]
    then
        make_workspaces=0
    fi
fi

##Check if user has modified number of parallel processes
if [ $# -gt 2 ]
then
    num_parallels=$3
fi

index=0
wspace_dir=${lim_dir}/workspaces
if [ ! -e $wspace_dir ]
then
    mkdir $wspace_dir
fi

# echo $wspace_dir
# echo $make_workspaces
# echo $2
# echo $num_parallels


if [ $make_workspaces != 0 ]
then 
    for file in $(ls -A ${lim_dir}/datacards/*.txt)
    do
        echo "Processing $file"
        filename=$(basename -- "$file")
        directory=$(dirname -- "$file")
        extension="${filename##*.}"
        filename="${filename%.*}"
        index=$((index+1))

        text2workspace.py ${file} -o ${wspace_dir}/${filename}.root --channel-masks &
        if (( $index % $num_parallels == 0 )) && [ $index -ne 0 ]
        then
            echo "Waiting for running jobs to finish..."
            wait
        fi
    done
    sleep 1
fi
index=0
rm ${lim_dir}/limits.txt
for file in $(ls -A ${lim_dir}/datacards/*.txt)
do
    #echo "Processing $file"
    filename=$(basename -- "$file")
    directory=$(dirname -- "$file")
    extension="${filename##*.}"
    filename="${filename%.*}"
    index=$((index+1))

    if ((${#channel_mask}>0))
    then
        ./scan_point.py -i $PWD/${wspace_dir}/${filename}.root -c ${channel_mask} &
    else
        ./scan_point.py -i $PWD/${wspace_dir}/${filename}.root &
    fi
    if (( $index % $num_parallels == 0 )) && [ $index -ne 0 ]
    then
        echo "Waiting for running jobs to finish..."
        wait
    fi
done

echo "Waiting for running jobs to finish..."
wait
