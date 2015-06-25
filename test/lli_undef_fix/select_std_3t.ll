; RUN: lit.py %s

; TODO: find some way to add lit code here to run this, and compare
;	its printed output against an expected value... which may be
;	affected by the intent that a poisoned value should crash this 
;	program when output.

; Verify that the select statement pass along poison from its third parameter
; when the selector is true.

; Declare the printf() control strings as global constants.
@unpoison_st = private unnamed_addr constant [21 x i8] c"unpoisoned: '0x%x' \0A\00"
@poison_st = private unnamed_addr constant [19 x i8] c"poisoned: '0x%x' \0A\00"

; External declaration of the printf function
declare i32 @printf(i8* nocapture readonly, ...)

; Definition of main function
define i32 @main() {   ; i32()*
  ; Convert [19 x i8]* to i8  *...
  %unpoison_st_i8 = getelementptr [21 x i8]* @unpoison_st, i64 0, i64 0
  %poison_st_i8 = getelementptr [19 x i8]* @poison_st, i64 0, i64 0

  %selector= sub i1 0, 1
  
  %unpoisoned_3= add i8 125, 5
  %poisoned_3= add nsw i8 125, 5

  %second= add i8 17, 3

  %unpoisoned_result1= select i1 %selector, i8 %second, i8 %unpoisoned_3
  %unpoisoned_result2= select i1 %selector, i8 %second, i8 %poisoned_3

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i8 %unpoisoned_result1 )
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i8 %unpoisoned_result2 )

  ; clean up and return
  ret i32 0
}

