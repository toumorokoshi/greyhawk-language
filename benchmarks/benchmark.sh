# this should run from the root of the git repo
echo "benchmarking greyhawk $1..."

COUNT=100
SCRIPT=$1

if [ -e "./benchmarks/$SCRIPT/$SCRIPT.c" ]; then
echo
echo
echo "C:"
clang ./benchmarks/$SCRIPT/$SCRIPT.c -o ./benchmarks/$SCRIPT/c_$SCRIPT
time {
    for run in {1..$COUNT}; do
        ./benchmarks/$SCRIPT/c_$SCRIPT > /dev/null
    done
}
fi

if [ -e "./benchmarks/$SCRIPT/$SCRIPT.gh" ]; then
echo
echo
echo "Greyhawk:"
time {
    for run in {1..$COUNT}; do
        ./bin/greyhawk ./benchmarks/$SCRIPT/$SCRIPT.gh > /dev/null
    done
}
fi

if [ -e "./benchmarks/$SCRIPT/$SCRIPT.py" ]; then
echo
echo
echo "Python:"
time {
    for run in {1..$COUNT}; do
      python ./benchmarks/$SCRIPT/$SCRIPT.py > /dev/null
    done
}
fi

if [ -e "./benchmarks/$SCRIPT/$SCRIPT.llvm" ]; then
    echo
    echo
    echo "LLVM:"
    time {
        for run in {1..$COUNT}; do
            lli ./benchmarks/$SCRIPT/$SCRIPT.llvm > /dev/null
        done
    }
fi
