#!/bin/bash

# Run all of the .ll files under test/lli_undef_fix with the poison_eq_undef
# flag set, and check their exit statuses.  It is hoped this will be more
# effective than trying # to maintain a separate test script for every .ll
# file.

export LLI_LUF_OPTS=poison_eq_undef
source scripts/run_lli_undef_fix_lls.sh

