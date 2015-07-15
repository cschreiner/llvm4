#!/bin/sh
# Run all of the .ll files under test/lli_undef_fix, and check their
# exit statuses.  It is hoped this will be more effective than trying
# to maintain a separate test script for every .ll file.


for ii in `find ../test/lli_undef_fix -iname \*.ll | sort `; do
   echo '#####################################################################'
   echo found test .ll program: $ii

   grep -q 'TESTDRIVER-BASED: *failure *expected' $ii
   if [ $? -eq 0 ]; then
      failure_expected=true
   fi
   
   $PROJ_ROOT/admin/bin/llifi $ii
   ll_status=$?
   echo -n exit status=\"$ll_status\"
   if [ "$failure_expected" = "true" ]; then
      echo \(Expected nonzero, i.e. .ll program should end with an error.\) 
      if [ $ll_status -eq 0 ]; then
         echo "TEST FAILURE: unexpected success"
      fi
   else
      echo \(Expected zero, i.e. .ll program should end normally.\)
      if [ $ll_status -eq 0 ]; then
         echo "TEST FAILURE: unexpected failure"
      fi
   fi
done
