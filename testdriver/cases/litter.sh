#!/bin/sh
# Run all of the lit tests EXCEPT for those under lli_undef_fix.  Those will
# be run by another test case.

cd ..
# Note: We need to change suffixes of form "(574 of 15638)" into something
# 	repeatable, as lit does not run the tests in a consistent order.
# Note: sort -k determines which fields on the line are the sort key.  We
# 	don't sort by field 1 because it contains the words 'PASS' or 'FAIL', 
# 	we want to sort by the filename information coming later.  This makes 
#	it easy to tell if a test difference is because a .ll file that 
#	used to pass failed, or visa versa, as opposed to a new .ll file 
#	was added somehow.
../build/bin/llvm-lit test | grep -e ^PASS -e ^FAIL -e ^X -e ^UNSUPPORTED | \
      sed 's/[0-9][0-9]* of [0-9][0-9]*/xx of yy/' | sort -k 2,9 
lit_status=$?

echo lit exit status=\"$lit_status\"
exit 0

