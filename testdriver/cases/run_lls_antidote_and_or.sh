#!/bin/bash

# Run all of the .ll files under test/lli_undef_fix with antidote_and_or
# flag set, and check their exit statuses.  It is hoped this will be more
# effective than trying # to maintain a separate test script for every .ll
# file.

export LLI_LUF_OPTS=antidote_and_or
source scripts/run_lli_undef_fix_lls.sh

