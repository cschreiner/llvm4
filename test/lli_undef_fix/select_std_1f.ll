; RUN: lit.py %s

; TODO: find some way to add lit code here to run this, and compare
;	its printed output against an expected value... which may be
;	affected by the intent that a poisoned value should crash this 
;	program when output.

; verify that the select statement pass along poison from its first parameter
; when it is false.

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

  %unpoisoned_1= add i1 1, 1
  %poisoned_1= add nsw i1 1, 1

  %a= add i8 2, 3
  %b= add i8 4, 5

  %unpoisoned_result= select i1 %unpoisoned_1, i8 %a, i8 %b
  %poisoned_result= select i1 %poisoned_1, i8 %a, i8 %b

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i8 %unpoisoned_result )
  call i32 (i8*, ...)* @printf(i8* %poison_st_i8, i8 %poisoned_result )

  ; clean up and return
  ret i32 0
}

