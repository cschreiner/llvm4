; don't run this test until we get the lit commands sorted out.
; RUN: false
; XFAIL:
; END.
; =======================================================================

; RUN: %lli -force-interpreter %s | FileCheck

; TESTDRIVER-BASED: failure expected
;; Failure is expected because the last printf(~) prints a poison value.
; XFAIL: * 
; TODO: add CHECK: lines here for FileCheck


; TODO: find some way to add lit code here to run this, and compare
;	its printed output against an expected value... which may be
;	affected by the intent that a poisoned value should crash this 
;	program when output.

; Description:

; Test ability to propogate poison at a inttoptr instruction.

; Declare the printf() control strings as global constants.
@unpoison_st = private unnamed_addr constant [21 x i8] c"unpoisoned: '0x%x' \0A\00"
@poison_st = private unnamed_addr constant [19 x i8] c"poisoned: '0x%x' \0A\00"

; External declaration of the puts function
declare i32 @printf(i8* nocapture readonly, ...)

; -----------------------------------------------------------------------------
define i32 @main() {   ; i32()*
; -----------------------------------------------------------------------------
  ; Convert [19 x i8]* to i8  *...
  %unpoison_st_i8 = getelementptr [21 x i8], [21 x i8]* @unpoison_st, i64 0, i64 0
  %poison_st_i8 = getelementptr [19 x i8], [19 x i8]* @poison_st, i64 0, i64 0

  ; . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
  ; test various combinations
  ; printfs that should NOT generate poision use printf with unpoison_st.
  ; printfs that should generate poision use printf with poison_st.

  %result0= inttoptr i19 5 to i18*
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i18* %result0 ) 

  %result1= inttoptr i32 78877 to i26*
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i26* %result1 ) 

  %result2= inttoptr i15 9 to i26*
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i26* %result2 ) 

  ; TODO2: when we get the ability to store a poisoned pointer value,
  ;  use that as an operand to ptrtoint instead of artificially adding
  ;  poison to the integer.
  %tmp3= add nsw nuw i18 %523, 262143 ; 262143 is INT20_MAX= 2**18-1
  %result3= inttoptr i18 %tmp3 to i21*
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i21* %result3 ) 

  ; . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
  ; clean up and return
  ret i32 0
}

