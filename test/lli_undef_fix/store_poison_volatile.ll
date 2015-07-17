; don't run this test until we get the lit commands sorted out.
; RUN: false
; XFAIL:
; END.
; =======================================================================

; RUN: %lli -force-interpreter %s | FileCheck

;; Failure is expected because the last store has a poisoned operand.
; TESTDRIVER-BASED: failure expected
; XFAIL: * 
; TODO: add CHECK: lines here for FileCheck


@printf_st = private unnamed_addr constant [37 x i8] c"this prints if poison-free: '0x%x' \0A\00"

; external declaration of the printf(~) function 
declare i32 @printf(i8* nocapture readonly, ...)

@addr_hxyp = global i11 1182

define i11 @main() { ; 
  ; %convert [? x i8]* to i8* 
  %printf_st_i8 = getelementptr [37 x i8], [37 x i8]* @printf_st, i64 0, i64 0

  %1= sub nuw i11 0, 1 ; generates POISON 
  store volatile i11 %1, i11* @addr_hxyp
  %2= add i16 0, 0 ; not poisoned

  call i32 (i8*, ...) @printf(i8* %printf_st_i8, i16 %2)

  ; clean up and return 
  ret i11 0 
} 

; end of program 

