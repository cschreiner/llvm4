; RUN lli -force-interpreter %s

@printf_st = private unnamed_addr constant [37 x i8] c"this prints if poison-free: '0x%x' \0A\00"

; external declaration of the printf(~) function 
declare i32 @printf(i8* nocapture readonly, ...)

@addr_a = global i11 1182

define i11 @main() { ; 
  ; %convert [? x i8]* to i8* 
  %printf_st_i8 = getelementptr [37 x i8]* @printf_st, i64 0, i64 0

  %1= sub i11 15, 1; not poisoned
  store i11 %1, i11* @addr_a;
  %2= load i11* @addr_a; 

  ; should print fine
  call i32 (i8*, ...)* @printf(i8* %printf_st_i8, i11 %2)

  %poison= sub nuw i11 0, 1 ; generates POISON 
  store i11 %poison, i11* @addr_a
  %poison2= load i11* @addr_a;

  ; should not print, as this is poisoned
  call i32 (i8*, ...)* @printf(i8* %printf_st_i8, i11 %poison2)

  ; clean up and return 
  ret i11 0 
} 

; end of program 

