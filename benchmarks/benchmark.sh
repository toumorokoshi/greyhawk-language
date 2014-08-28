# this should run from the root of the git repo
echo "benchmarking greyhawk $1..."

COUNT=100
SCRIPT=$1

echo
echo
echo "Greyhawk:"
time {
    for run in {1..$COUNT}; do
      ./bin/greyhawk ./benchmarks/$SCRIPT/$SCRIPT.gh > /dev/null
    done
}

echo
echo
echo "Python:"
time {
    for run in {1..$COUNT}; do
      python ./benchmarks/$SCRIPT/$SCRIPT.py > /dev/null
    done
}
