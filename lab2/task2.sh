#!bin/bash
  function get_word_by_index() {
    eval text="$1"
    read -ra arr <<< "$text"
    if [[ ${#arr[@]} -eq 0 ]]; then
      echo "Entered string is empty" >&2; exit 1
    fi
    echo "Enter number of field:"
    read num
    re='^[0-9]\d*$'
    if ! [[ $num =~ $re ]] ; then
      echo "Invalid number" >&2; exit 1
    fi
    arr_len="${#arr[@]}"
    if [[ $((arr_len - 1)) -lt $num ]]; then
      echo "Too large number" >&2; exit 1
    fi
    echo "Field with index ${num}: ${arr[num]}"
  }

get_word_by_index
