#! /bin/bash
echo "Enter two numbers:"
read num1 num2
re='^[+-]?[0-9]+([.][0-9]+)?$'
if ! [[ $num1 =~ $re ]] ; then
echo "error: first entered number is not a number" >&2; exit 1
fi
if ! [[ $num2 =~ $re ]] ; then
echo "error: second entered number is not a number" >&2; exit 1
fi
echo "Choose operation by number:
1) +
2) -
3) *
4) /
"
read op
case "$op"
in
1) expr $num1 + $num2
;;
2) expr $num1 - $num2
;;
3) expr $num1 \* $num2
;;
4) if (($num2 == 0)); then
echo "Division by 0"
else
bc -l <<< "scale = 5; ${num1}/${num2}"
fi
;;
*) echo "error: unknown command number" >&2; exit 1
esac
