#!/bin/bash

# Run all of the .ll files under test/lli_undef_fix with the nuno flag set,
# and check their exit statuses.  It is hoped this will be more effective
# than trying # to maintain a separate test script for every .ll file.


export LLI_LUF_OPTS=nuno
source scripts/run_lli_undef_fix_lls.sh

