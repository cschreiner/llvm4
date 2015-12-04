#!/bin/bash

# Run all of the .ll files under test/lli_undef_fix with no flags set, and
# check their exit statuses.  It is hoped this will be more effective than
# trying # to maintain a separate test script for every .ll file.


source scripts/run_lli_undef_fix_lls.sh

