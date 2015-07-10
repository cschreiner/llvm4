; RUN: lit.py %s

; TODO: find some way to add lit code here to run this, and compare
;	its printed output against an expected value... which may be
;	affected by the intent that a poisoned value should crash this 
;	program when output.

; ashr numbers with both 0 and 1 bits being shifted out, usually
;  under conditions that don't generate poison, finally under
;  conditions that do.

; Declare the printf() control strings as global constants.
@unpoison_st = private unnamed_addr constant [21 x i8] c"unpoisoned: '0x%x' \0A\00"
@poison_st = private unnamed_addr constant [19 x i8] c"poisoned: '0x%x' \0A\00"

; External declaration of the puts function
declare i32 @printf(i8* nocapture readonly, ...)

; Definition of main function
define i32 @main() {   ; i32()*
  ; Convert [19 x i16]* to i16  *...
  %unpoison_st_i8 = getelementptr [21 x i8]* @unpoison_st, i64 0, i64 0
  %poison_st_i8 = getelementptr [19 x i8]* @poison_st, i64 0, i64 0

  ; 58016 == 0xe2a0  == ... 1110 0010 1010 0000
  ; right shift by 3 == 1111 1100 0101 0100 == 0xfc54
  %nowrap1= ashr i16 58016, 3; 
  %nowrap2= ashr exact i16 58016, 3 ; 

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap1 )
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap2 )

  ; 13333 == 0x3415  == ... 0011 0100 0001 0101
  ; right shift by 3 == 0000 0110 1000 0010 == 0x0682
  %unpoisoned_1= ashr i16 13333, 3 ; 
  %poisoned_1= ashr exact i16 13333, 3 ; 

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %unpoisoned_1 )
  call i32 (i8*, ...)* @printf(i8* %poison_st_i8, i16 %poisoned_1 )

  ; clean up and return
  ret i32 0
}

