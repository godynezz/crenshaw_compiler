#!/bin/bash
#download all parts
errors=0
for i in $(seq 1 16); do
    file="tutor"$i".txt"
    echo -ne "                           \r"
    echo -ne "downloading $file\r"
    wget "http://compilers.iecc.com/crenshaw/"$file > /dev/null 2> /dev/null
    code=$?

    if [ $code != 0 ]; then
        echo -ne "                           \r"
        echo "$file: download error, code -> $code"
        errors=$((errors + 1))
    else
        echo -ne "                           \r"
        echo "$file: download complete"
    fi

done;

if (( $errors > 0 )); then
    echo "there were $errors errors on the download process"
    exit 1
else 
    echo "all parts downloaded!"
fi
