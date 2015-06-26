; RUN: llvm-as < %s 2>&1 | FileCheck %s 

; CHECK-NOT: Instruction operands must be first-class

;; Test that a structure can be stored to memory.  This test fails with lli
;; from svn revision 240238: 
;;	lli 2015-02-25-StoreStruct.ll
;;

%struct_4 = type { i32 }

@some_addr= global %struct_4 { i32 5 }, align 8

define void @main() {
  store %struct_4 zeroinitializer, %struct_4* @some_addr
  ret void
}
