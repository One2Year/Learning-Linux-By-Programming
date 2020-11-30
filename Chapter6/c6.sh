#
cc rotate.c -o rotate
./rotate
# <- is backspace
# abx<-cd
# bcde
# efg Ctrl-c

stty -icanon; ./rotate

stty -icanon -echo; ./rotate

stty icanon echo