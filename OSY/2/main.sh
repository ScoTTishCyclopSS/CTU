#!/bin/bash
#author: @timusfed

exitCode=0 # default exit code
id=0 # array stuff, huh
z=0

# === Additional functions ===
errEcho () { >&2 echo "ERROR '$1'"; exitCode=1; } # writes an error
countFiles () { count=$(wc -l < "$1") || exit 2; } # counts files in the directory
zipFiles () { tar -czf output.tgz "${array[@]}" || exit 2; } # zip files from an array
defineType () 
{ # -d for dir, -f for file, -p for path (link)
    if [[ -L $1 ]]; then
        echo "LINK '$1' '$(readlink "$1")'"
    elif [[ -f $1 ]]; then
        countFiles "$1"
        array[$id]=$1
        id=$((id+1))
        echo "FILE '$1' $count '$(head -n 1 "$1")'"
    elif [[ -d $1 ]]; then
        echo "DIR '$1'" 
    else
        errEcho "$1"
    fi
}

# === Read the income params ===
while getopts "hz" opt; do
    case $opt in
        h) echo "Script by @timusfed."; echo "Checks input paths, that's all."; exit 0;; # after -h exit
        z) z=1;;
        ?) exitCode=2; exit $exitCode;;
    esac
done

# === The main structure ===
while read -r Path
do
    if [ "${Path:0:5}" = "PATH " ]; then #path:offset:how_much
        path=${Path:5}
        defineType "$path"
    fi
done
if [ "$z" = 1 ]; then
    zipFiles
fi
exit $exitCode
