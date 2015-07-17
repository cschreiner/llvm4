#!/bin/sh
# Run all of the lit tests EXCEPT for those under lli_undef_fix.  Those will
# be run by another test case.

cd ..
# We need to change suffixes of form "(574 of 15638)" into something
# repeatable, as lit does not run the tests in a consistent order.
../build/bin/llvm-lit test | grep -e ^PASS -e ^FAIL -e ^X -e ^UNSUPPORTED | \
      sed 's/[0-9][0-9]* of [0-9][0-9]*/xx of yy/' | sort 
lit_status=$?

echo lit exit status=\"$lit_status\"
exit 0

