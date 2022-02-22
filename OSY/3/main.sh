#!/bin/bash
#author: @timusfed

pdf="false"
textInputCounted="false"
textInputSentence="false"
#inputSourceCode="false"

# === Additional functions ===
pdfFinder () { # finds all *.pdf files and show them sorted by name
    find . -type f -iname "*.pdf" -printf "%f\n" | sort -n
}

digitRowFinder () { # if row starts with number - show it
    sed "s,^[+-]\?[0-9]\+,,; t; d;"
}

sentenceFinder () { # finds sentences
    IFS=''
    comp=$(tr '\n' ' ') # sentence starts with capital letter and continues until .?! + at the end must be .?!
    echo "$comp" | grep -o '[[:upper:]][^.!?]*[.!?]'
}

inputFinder () { # finds inputs in .c and changes them
    perl -pe 's,(#[ \t]*include[ \t]*)((")([A-Za-z.]+)(")|(<)([A-Za-z.]+)(>)),\1\3\6'"$1"'\4\7\5\8,'
}

# === Read the income params ===
while getopts "habcd" opt; do
    case $opt in
        h) echo "It's a private @timusfed's script. You are not allowed here."; echo "Bravo-Six. Going dark."; exit 0;; # after -h exit
        a) pdf="true";;
        b) textInputCounted="true";;
        c) textInputSentence="true";;
        d) inputSourceCode="true";;
        ?) echo "You didn't write any flag to @timusfed."; echo "Bravo-Six. Going dark."; exit 1;;
    esac
done

# === The main structure ===
if [ $pdf = "true" ]; then
    pdfFinder
elif [ $textInputCounted = "true" ]; then
    digitRowFinder
elif [ $textInputSentence = "true" ]; then
    sentenceFinder
elif [ $inputSourceCode = "true" ]; then
    inputFinder "$2"
fi
exit 0
