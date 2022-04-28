build_tests() {
        for test_source in $TEST_SOURCES; do
                if test -f $test_source; then
                        $CC $LDFLAGS $CFLAGS -o $(dirname $test_source)/$(basename $test_source .c) $test_source $SOURCES
                fi
        done
}

run_tests() {
        echo "Running unit tests:"

        for test in $TESTS; do
                if test -f $test; then
                        if $VALGRIND ./$test 2>> tests/tests.log; then
                                echo $test PASS
                        else
                                echo "ERROR in test $test: here's tests/tests.log"
                                echo "-----"
                                tail tests/tests.log
                                exit 1
                        fi
                fi
        done

        echo "Done"
}

build_tests
run_tests
