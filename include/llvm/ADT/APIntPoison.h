//===-- llvm/ADT/APIntPoison.h - Arbitrary Precision Integer Poison issues --*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file implements a namespace of functions that detect when an
///   APInt has become poisoned. 
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_ADT_APINTPOISON_H
#define LLVM_ADT_APINTPOISON_H

#include <cassert>
#include <climits>
#include <cstring>
#include <string>
#include "llvm/IR/Value.h"

#include "llvm/ADT/APInt.h"

namespace llvm {

//===----------------------------------------------------------------------===//
//                              APIntPoison Namespace
namespace APIntPoison {


/* ===========================================================================
   DISPATCHED FUNCTIONS: There should be a function here for each LLVM
   instruction that has anything in any sense to do with poison.  A
   couple instructions, like udiv and sdiv, share an function.
 */
// TODO2: see if more const designations can be added to parameters.

// Note: add overloads here as needed, for example, one operand
// may be an integer (signed or not).

// ---------------------------------------------------------------------------
// macro DISPATCH_FUNCTIONS_FOR_SCHEME
// ---------------------------------------------------------------------------
// \brief quick way to generate all of the prototypes for a scheme's
// dispatched functons.
//
// \param SCHEME: the name of the scheme to use in expansion.  SCHEME 
//	values usually need to begin with an underscore.  
//
// Example: DISPATCH_FUNCTIONS_FOR_SCHEME(_SchemeEtc)

#define DISPATCH_FUNCTIONS_FOR_SCHEME( SCHEME )				\
  void poisonIfNeeded_add#SCHEME( APInt& dest, APInt& lhs, APInt& rhs,	\
				  bool nsw, bool nuw );			\
  /* Looks like we need separate checks for ++ vs a full add.		\
     Add such a function if needed.					\
  */									\
  void poisonIfNeeded_sub#SCHEME( APInt& dest, APInt& lhs, APInt& rhs,	\
				  bool nsw, bool nuw );			\
  void poisonIfNeeded_mul#SCHEME( APInt& dest, APInt& lhs, APInt& rhs,	\
				  bool nsw, bool nuw );			\
									\
  void poisonIfNeeded_div#SCHEME( APInt& dest, APInt& lhs, APInt& rhs,	\
				  bool exact );				\
  void poisonIfNeeded_div#SCHEME( APInt& dest, APInt& lhs, APInt& rhs ); \
  void poisonIfNeeded_rem#SCHEME( APInt& dest, APInt& lhs, APInt& rhs ); \
									\
  void poisonIfNeeded_bitAnd#SCHEME( APInt& dest,			\
				     const APInt& lhs, const APInt& rhs ); \
  void poisonIfNeeded_bitOr#SCHEME( APInt& dest,			\
				    const APInt& lhs, const APInt& rhs ); \
  void poisonIfNeeded_bitXor#SCHEME( APInt& dest,			\
				     const APInt& lhs, const APInt& rhs ); \
									\
  void poisonIfNeeded_shl#SCHEME( APInt& dest, APInt& src,		\
				  unsigned shiftAmt, bool nsw, bool nuw ); \
  void poisonIfNeeded_lshr#SCHEME( APInt& dest, APInt& src,		\
				   unsigned shiftAmt, bool exact );	\
  void poisonIfNeeded_ashr#SCHEME( APInt& dest, APInt& src,		\
				   unsigned shiftAmt, bool exact );	\
									\
  void poisonIfNeeded_select#SCHEME( APInt& dest,			\
				     const APInt& src1, const APInt& src2, \
				     const APInt& src3 );		\
									\
  void poisonIfNeeded_trunc#SCHEME( APInt& dest, const APInt& src,	\
				    const unsigned newBitWidth );	\
  void poisonIfNeeded_sext#SCHEME( APInt& dest, const APInt& src,	\
				   const unsigned newBitWidth );	\
  void poisonIfNeeded_zext#SCHEME( APInt& dest, const APInt& src,	\
				   const unsigned newBitWidth );	\
									\
  void poisonIfNeeded_ptrtoint#SCHEME( APInt& dest, const APInt& src );	\
  void poisonIfNeeded_inttoptr#SCHEME( APInt& dest, const APInt& src );	\
  void poisonIfNeeded_bitcast#SCHEME( APInt& dest, const APInt& src );	\
  void poisonIfNeeded_icmp#SCHEME( APInt& dest,				\
				   const APInt& lhs, const APInt& rhs ); \
  //  Note: phi and call are handled by normal poison propogation in APInt \
  void poisonIfNeeded_br#SCHEME();					\
									\
  void poisonIfNeeded_getelementptr#SCHEME( Value& dest,		\
					    APInt& lhs, APInt& rhs,	\
					    bool inbounds );

public:
  DISPATCH_FUNCTIONS_FOR_SCHEME();
private:
  DISPATCH_FUNCTIONS_FOR_SCHEME(_SchemeEtc);
  DISPATCH_FUNCTIONS_FOR_SCHEME(_SchemeNuno);

#undef DISPATCH_FUNCTIONS_FOR_SCHEME

/* ===========================================================================
   END OF DISPATCHED FUNCTIONS
*/

public:
void printIfPoison( Instruction& In, APInt& val );
// TODO2: see if const qualifiers can be applied to I or val

} // End of APIntPoison namespace

} // End of llvm namespace

#endif
