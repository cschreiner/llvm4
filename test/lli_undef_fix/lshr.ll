; RUN: lit.py %s

; TODO: find some way to add lit code here to run this, and compare
;	its printed output against an expected value... which may be
;	affected by the intent that a poisoned value should crash this 
;	program when output.

; lshr numbers with both 0 and 1 bits being shifted out, usually
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

  ; These don't shift out any bits that would generate poison.
  ; 44256 == 0xace0  == . 1010 1100 1110 0000
  ; right shift by 2 == 0010 1011 0011 1000  == 0x2b38
  %nowrap1= lshr i16 44256, 2 ;
  %nowrap2= lshr exact i16 44256, 2 ; 

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap1 )
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap2 )

  ; 52827 == 0xce5b  == . 1100 1110 0101 1011
  ; right shift by 2 == 0011 0011 1001 0110 == 0x3396
  %unpoisoned_1= lshr i16 52827, 2 ; 
  %poisoned_1= lshr exact i16 52827, 2 ; 

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %unpoisoned_1 )
  call i32 (i8*, ...)* @printf(i8* %poison_st_i8, i16 %poisoned_1 )

  ; clean up and return
  ret i32 0
}

