#!/bin/sh
# Run all of the lit tests EXCEPT for those under lli_undef_fix.  Those will
# be run by another test case.

cd ..
../build/bin/llvm-lit test 
lit_status=$?

echo lit exit status=\"$lit_status\"
exit 0

