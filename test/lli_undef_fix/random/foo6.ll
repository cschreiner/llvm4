; autogenerated by generate_llvm_assembler.pl. 

@printf_st = private unnamed_addr constant [37 x i8] c"this prints if poison-free: '0x%x' \0A\00"

; external declaration of the printf(~) function 
declare i32 @printf(i8* nocapture readonly, ...)

@addr_minj = global i29 394499692

define i29 @main() { ; 
  ; %convert [? x i8]* to i8* 
  %printf_st_i8 = getelementptr [37 x i8], [37 x i8]* @printf_st, i64 0, i64 0

  %1= add i29 92473922, 0 
  %2= add i29 214324235, 0 
  %3= sub nuw i29 0, 1 ; generates POISON 
  store volatile i29 %3, i29* @addr_minj
  %4= load i29* @addr_minj 
  %5= udiv i29 %4, %2
  %6= ashr i29 %5, 7
  %7= urem i29 %2, %6
  %8= shl nuw i29 %7, 16
  %9= xor i29 %8, %3
  %10= udiv exact i29 %9, 324733706
  %11= mul nsw nuw i29 %7, %10
  %12= sub i29 240553118, %11
  %13= udiv exact i29 %12, 497949

  call i32 (i8*, ...) @printf(i8* %printf_st_i8, i29 %13)

  ; clean up and return 
  ret i29 0 
} 

; end of program 

