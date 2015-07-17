#!/bin/sh
# Run all of the .ll files under test/lli_undef_fix, and check their
# exit statuses.  It is hoped this will be more effective than trying
# to maintain a separate test script for every .ll file.

echo2()
{
   echo "$@"
   echo "$@" >> /dev/stderr
}


for ii in `find ../test/lli_undef_fix -maxdepth 1 -iname \*.ll | sort `; do
   echo2 '#####################################################################'
   echo2 found test .ll program: $ii

   failure_expected=false
   grep -qE 'TESTDRIVER-BASED: *failure *expected\b' $ii
   if [ $? -eq 0 ]; then
      failure_expected=true
      echo2 "  " .ll file says failure expected
   else
      echo2 "  " .ll file says success expected
   fi

   echo2 " "
   sync
   
   # run the .ll file
   # we run it twice, once to capture stdout, another time to capture stderr.
   $PROJ_ROOT/admin/bin/llifi $ii 2>> /dev/null
   stdbuf -o 0 -e 0 $PROJ_ROOT/admin/bin/llifi $ii 2>&1 1>> /dev/null | \
   	 sed 's/addr=0x[0-9a-zA-Z]*/addr=dummy_address/' 1>&2

   # Note: this didn't work
   #(stdbuf -o 0 -e 0 $PROJ_ROOT/admin/bin/llifi $ii 2>&3 ) 3>&1 1>&2 | \
   #	 sed 's/addr=0x[0-9a-zA-Z]*/addr=dummy_address/'
   ll_status=$?
   echo -n exit status=\"$ll_status\" " "

   # process the results
   if [ "$failure_expected" = "true" ]; then
      echo \(Expected nonzero, i.e. .ll program should end with an error.\) 
      if [ $ll_status -eq 0 ]; then
         echo "TEST FAILURE: unexpected .ll success"
      fi
   else
      echo \(Expected zero, i.e. .ll program should end normally.\)
      if [ $ll_status -ne 0 ]; then
         echo "TEST FAILURE: unexpected .ll failure"
      fi
   fi
   echo " "
done
