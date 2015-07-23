#!/bin/bash

# Run all of the .ll files under test/lli_undef_fix with the return_if_div_0
# flag set, and check their exit statuses.  It is hoped this will be more
# effective than trying # to maintain a separate test script for every .ll
# file.

export LLI_LUF_OPTS=return_if_div_0
source scripts/run_lli_undef_fix_lls.sh

