; RUN: lit.py %s

; TODO: find some way to add lit code here to run this, and compare
;	its printed output against an expected value... which may be
;	affected by the intent that a poisoned value should crash this 
;	program when output.

; shl signed numbers with both 0 and 1 bits being shifted out, usually
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
  ; 65297 == 0xff11 == 1111 1111 0001 0001
  ; left shift by 7 == .... ... 1000 1000 1000 0000 == 0x8880
  %nowrap1= shl i16 65297, 7; was 178, 7
  %nowrap2= shl nsw i16 65297, 7 ; was 178, 7

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap1 )
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %nowrap2 )

  ; 61186 == 0xef02 == 1110 1111 0000 0010
  ; shift left by 7 == .... ... 1000 0001 0000 0000 == 0x8100
  %unpoisoned_1= shl i16 61186, 7 ; was 122, 7
  %poisoned_1= shl nsw i16 61186, 7 ; was 122, 7

  ; Call puts function to write out the string to stdout.
  call i32 (i8*, ...)* @printf(i8* %unpoison_st_i8, i16 %unpoisoned_1 )
  call i32 (i8*, ...)* @printf(i8* %poison_st_i8, i16 %poisoned_1 )

  ; clean up and return
  ret i32 0
}

