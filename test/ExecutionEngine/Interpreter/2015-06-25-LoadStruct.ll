; RUN: %lli -force-interpreter %s 

;; Test that a structure can be loaded from memory.  

;; Note: This test fails with `lli -force-interpreter` from svn revision 
;; 240238: 
;;	lli -force-interpreter 2015-02-25-StoreStruct.ll
;; but succeeds with the accompanying patch.

%struct_4 = type { i32 }

@some_addr= global %struct_4 { i32 5 }, align 8

define void @main() {
  %1= load %struct_4, %struct_4* @some_addr
  ret void
}
