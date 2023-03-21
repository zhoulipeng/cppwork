for i in `find . -type f -name "*+*.mp3"`
do 
    text = `echo $i | sed -e 's/+//g'`
    echo "$i $text" >> $$.log
    if [ -e $text ]; then
        echo "$text exists"
        break
    else
        mv $i $text
        if [ $? -ne 0 ]; then
            echo "$i $text failed"
            break
        fi
    fi

done

# find 批量改名2
find Texture -name AdvancedLotteryCheckBox.png -exec bash -c 'mv "$1" "${1/Advanced/DemoAv}"  ' -- {} \;
