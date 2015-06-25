; ModuleID = 'small.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.S2 = type { i32, i32, i32 }

@mercury_printf_st= private unnamed_addr constant [16 x i8] c"got to mercury\0A\00"
@i96_printf_st= private unnamed_addr constant [15  x i8] c"struct={%llx}\0A\00"

@a = common global %struct.S2 zeroinitializer, align 4

; Eternal declaration of printf()
declare i32 @printf(i8* nocapture readonly, ...)

; Function Attrs: nounwind uwtable
define { i64, i32 } @fn1() #0 {
entry:
  %mercury_printf_st_i8= getelementptr [16 x i8]* @mercury_printf_st, i64 0, i64 0
  %i96_printf_st_i8= getelementptr [15 x i8]* @i96_printf_st, i64 0, i64 0
  %retval = alloca %struct.S2, align 4
  %tmp = alloca { i64, i32 }
  %0 = bitcast %struct.S2* %retval to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast (%struct.S2* @a to i8*), i64 12, i32 4, i1 false)
  %1 = bitcast { i64, i32 }* %tmp to i8*
  %2 = bitcast %struct.S2* %retval to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* %2, i64 12, i32 1, i1 false)
  call i32 (i8*,...)* @printf( i8* %mercury_printf_st_i8 ) ;;
  %4 = load { i64, i32 }* %tmp
  %reg3_field0= extractvalue { i64, i32 } %4, 0 ;;
  %reg3_field1= extractvalue { i64, i32 } %4, 1 ;;
  call i32 (i8*, ...)* @printf( i8* %i96_printf_st_i8, i64 %reg3_field0 ) ;;
  ret { i64, i32 } %4
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %coerce = alloca %struct.S2, align 4
  %tmp = alloca { i64, i32 }
  store i32 0, i32* %retval
  %call = call { i64, i32 } @fn1()
  store { i64, i32 } %call, { i64, i32 }* %tmp
  %0 = bitcast { i64, i32 }* %tmp to i8*
  %1 = bitcast %struct.S2* %coerce to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* %0, i64 12, i32 1, i1 false)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.6.0 (trunk 217983)"}
