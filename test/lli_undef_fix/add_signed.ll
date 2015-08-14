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

; Add signed numbers together, twice under conditions should not generate
; poison, once under conditions that would generate poison if banned (but
; it isn't banned), once that generates poison.

; Declare the printf() control strings as global constants.
@unpoison_st = private unnamed_addr constant [21 x i8] c"unpoisoned: '0x%x' \0A\00"
@poison_st = private unnamed_addr constant [19 x i8] c"poisoned: '0x%x' \0A\00"

; External declaration of the puts function
declare i32 @printf(i8* nocapture readonly, ...)

; Definition of main function
define i32 @main() {   ; i32()*
  ; Convert [19 x i8]* to i8  *...
  %unpoison_st_i8 = getelementptr [21 x i8], [21 x i8]* @unpoison_st, i64 0, i64 0
  %poison_st_i8 = getelementptr [19 x i8], [19 x i8]* @poison_st, i64 0, i64 0

  %nowrap1= add i8 178, 7
  %nowrap2= add nsw i8 178, 7

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i8 %nowrap1 )
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i8 %nowrap2 )

  %unpoisoned_1= add i8 122, 7
  %poisoned_1= add nsw i8 122, 7

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...) @printf(i8* %unpoison_st_i8, i8 %unpoisoned_1 )
  call i32 (i8*, ...) @printf(i8* %poison_st_i8, i8 %poisoned_1 )

  ; clean up and return
  ret i32 0
}

