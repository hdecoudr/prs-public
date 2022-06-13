# Use ./sigcprocmask <pid> in order to see the signals handled by a process.

function use {
    printf "Use:\n  $0 <target_pid>\n"
}

function sigparse {
    i=0
    bits="$(printf "ibase=16; obase=2; %X\n" "0x$1" | bc)"
    while [ -n "$bits" ] ; do
        i="$(expr "$i" + 1)"
        case "$bits" in
            *1) printf " %s(%s)" "$(kill -l "$i")" "$i" ;;
        esac
        bits="${bits%?}"
    done
}

if [ "$#" -ne 1 ] ; then
    use
    exit 1
fi 

grep "^Sig...:" "/proc/$1/status" | while read a b ; do
    printf "%s%s\n" "$a" "$(sigparse "$b")"
done
    
