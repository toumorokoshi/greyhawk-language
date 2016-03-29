GTEST_URL=https://googletest.googlecode.com/files/gtest-1.7.0.zip
GTEST_FOLDER=gtest-1.7.0

if [ ! -e ./gtest ]; then
    echo "installing gtest..."
    wget $GTEST_URL -O /tmp/gtest.zip
    unzip /tmp/gtest.zip -d .
    mv $GTEST_FOLDER gtest
    cd ./gtest
    ./configure > /dev/null || echo "gtest: there was an error running ./configure!";
    make > /dev/null || echo "gtest: there was an error running make!"
fi
