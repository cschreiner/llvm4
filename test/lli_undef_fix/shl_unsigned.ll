; RUN: lit.py %s

; TODO: find some way to add lit code here to run this, and compare
;	its printed output against an expected value... which may be
;	affected by the intent that a poisoned value should crash this 
;	program when output.

; shl unsigned numbers with both 0 and 1 bits being shifted out, usually
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

  ; these don't shift out any bits that would generate poison.
  ; 1323 == 0x052b == 0000 0101 0010 1011 
  ; left shift by 4 == .... 0100 0010 1011 0000 == 52b0
  %nowrap1= shl i16 1323, 4; 
  %nowrap2= shl nuw i16 1323, 4 ; 

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap1 )
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap2 )

  ; 52093 == 0xcb7d == 1100 1011 0111 1101
  ; shift left by 5 == .... .0110 1111 1010 0000 == 0x6fa0
  %unpoisoned_1= shl i16 52093, 5 ; 
  %poisoned_1= shl nuw i16 52093, 5 ; 

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %unpoisoned_1 )
  call i32 (i8*, ...)* @printf(i8* %poison_st_i8, i16 %poisoned_1 )

  ; clean up and return
  ret i32 0
}

