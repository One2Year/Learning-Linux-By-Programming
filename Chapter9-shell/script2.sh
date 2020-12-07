# ! /bin/sh
# script2.c - a real program with variables, input and control flow
# bug: script2.sh: 4: BOOK: not found

BOOK = $HOME/phonebook.data
echo find what name in phonebook
read NAME
if grep $NAME $BOOK > /tmp/pb.tmp
then
    echo Entries for $NAME
    cat /tmp/pb.tmp
else 
    echo No entires for $NAME
fi
rm /tmp/pb.tmp
