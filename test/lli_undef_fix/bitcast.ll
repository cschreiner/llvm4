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

; Test ability to propogate poison at a bitcast instruction.

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
  ; printfs that should NOT generate poison use printf with unpoison_st.
  ; printfs that should generate poison use printf with poison_st.

  %a0= bitcast i7 0 to i7
  %result0= bitcast i7 %a0 to i7
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i7 %result0 ) 

  %a1= bitcast i30 50928 to i30 ; 50928=0xc6f0
  %result1= bitcast i30 %a1 to i30
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i30 %result1 ) 

  %a2= bitcast i85 9 to i85
  %result2= bitcast i85 %a2 to i85
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i85 %result2 ) 

  %poison3= add nsw nuw i5 31, 26; 
  %a3= bitcast i5 %poison3 to i5
  %result3= bitcast i5 %a3 to i5
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i5 %result3 ) 

  ; . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
  ; clean up and return
  ret i32 0
}

