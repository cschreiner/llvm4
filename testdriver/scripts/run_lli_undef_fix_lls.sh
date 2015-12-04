#!/bin/bash
# Run all of the .ll files under test/lli_undef_fix, and check their
# exit statuses.  It is hoped this will be more effective than trying
# to maintain a separate test script for every .ll file.


# Note: some editors create a temporary file called .#x.ll (or similar) when
# editing file x.ll, this temporary file should not run as part of a test
# case.
for ii in `find ../test/lli_undef_fix -maxdepth 1 -iname \*.ll ! -iname .\* | sort `; do
   echo '#####################################################################'
   echo found test .ll program: $ii

   failure_expected=false
   grep -qE 'TESTDRIVER-BASED: *failure *expected\b' $ii
   if [ $? -eq 0 ]; then
      failure_expected=true
      echo "  " .ll file says failure expected
   else
      echo "  " .ll file says success expected
   fi

   echo " "
   sync
   
   # run the .ll file
   # We run it twice, once to capture stdout, another time to capture stderr.
   # Everything is eventually sent to stdout, so it can be compared with 
   # benchmarks in a predictable way.  Sheesh.
   $PROJ_ROOT/admin/bin/llifi $ii 2>> /dev/null
   ll_status1=$?
   echo -e '\n########'
   echo -e stderr: '\n'
   $PROJ_ROOT/admin/bin/llifi $ii 2>&1 1>> /dev/null | \
   	 sed 's/addr=0x[0-9a-zA-Z]*/addr=dummy_address/' 
   ll_status_array=(${PIPESTATUS[@]})
   ll_status2=${ll_status_array[0]}
   pipe_status=${ll_status_array[1]}

   # Note: this didn't work, neither did a dozen+ other attempts to avoid 
   # running the .ll program twice.  Sheesh.
   #(stdbuf -o 0 -e 0 $PROJ_ROOT/admin/bin/llifi $ii 2>&3 ) 3>&1 1>&2 | \
   #	 sed 's/addr=0x[0-9a-zA-Z]*/addr=dummy_address/'
   

   # process the results
   echo -e '\n########'
   if [ $pipe_status -ne 0 ]; then
      echo TEST FAILURE: stderr filter failed, exit status=\"$pipe_status\".
   fi
   if [ $ll_status1 -ne $ll_status2 ]; then
      echo TEST FAILURE: statuses disagree:
      echo "  " ll_status1=\"$ll_status1\", ll_status2=\"$ll_status2\"
   fi
   echo -n exit status=\"$ll_status1\" " "
   if [ "$failure_expected" = "true" ]; then
      echo \(Expected nonzero, i.e. .ll program should end with an error.\) 
      if [ $ll_status1 -eq 0 ]; then
         echo "TEST FAILURE: unexpected .ll success"
      fi
   else
      echo \(Expected zero, i.e. .ll program should end normally.\)
      if [ $ll_status1 -ne 0 ]; then
         echo "TEST FAILURE: unexpected .ll failure"
      fi
   fi
   echo " "
done
