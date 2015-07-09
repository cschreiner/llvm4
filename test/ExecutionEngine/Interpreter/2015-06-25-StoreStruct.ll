; RUN: %lli -force-interpreter %s 
;; we expect this to fail until the investigation mentioned below is finished.
; XFAIL: * 

;; Status: this test currently fails, but not for the intended reason.
;; Apparently while the raw "store" code works, some other part of the
;; interpreter gripes and halts.  Investigation is still pending as of
;; 2015jul08.

;; Test that a structure can be stored to memory.  

;; This test fails with lli from svn revision 240238, see the above note: 
;;	lli -force-interpreter 2015-02-25-StoreStruct.ll
;; The reason for failure changes with the accompanying patch.

%struct_4 = type { i32 }

@some_addr= global %struct_4 { i32 5 }, align 8

define void @main() {
  store %struct_4 zeroinitializer, %struct_4* @some_addr
  ret void
}

