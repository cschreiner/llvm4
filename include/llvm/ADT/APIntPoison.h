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

/* There should be exactly one function here for each LLVM instruction
   that can generate a poison value.
 */
// TODO: see if more const designations can be added to parameters.

// TODO: add overloads here as appropriate, for example, one operand
// may be an integer (signed or not).

void poisonIfNeeded_add( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw );
/* Looks like we need separate checks for ++ vs a full add.
   Add such a function if needed.
*/
void poisonIfNeeded_sub( APInt& dest, APInt& lhs, APInt& rhs, 
			   bool nsw, bool nuw );
void poisonIfNeeded_mul( APInt& dest, APInt& lhs, APInt& rhs, 
			   bool nsw, bool nuw );

void poisonIfNeeded_div( APInt& dest, APInt& lhs, APInt& rhs, bool exact );
void poisonIfNeeded_div( APInt& dest, APInt& lhs, APInt& rhs );
void poisonIfNeeded_rem( APInt& dest, APInt& lhs, APInt& rhs );

void poisonIfNeeded_bitAnd( APInt& dest, const APInt& lhs, const APInt& rhs );
void poisonIfNeeded_bitOr( APInt& dest, const APInt& lhs, const APInt& rhs );
void poisonIfNeeded_bitXor( APInt& dest, const APInt& lhs, const APInt& rhs );

void poisonIfNeeded_shl( APInt& dest, APInt& src, unsigned shiftAmt, 
    bool nsw, bool nuw );
void poisonIfNeeded_lshr( APInt& dest, APInt& src, unsigned shiftAmt, 
    bool exact );
void poisonIfNeeded_ashr( APInt& dest, APInt& src, unsigned shiftAmt, 
    bool exact );

void poisonIfNeeded_select( APInt& dest, 
    const APInt& src1, const APInt& src2, const APInt& src3 );

void poisonIfNeeded_trunc( APInt& dest, const APInt& src, 
    const unsigned newBitWidth );
void poisonIfNeeded_sext( APInt& dest, const APInt& src, 
    const unsigned newBitWidth );
void poisonIfNeeded_zext( APInt& dest, const APInt& src, 
    const unsigned newBitWidth );


// CAS TODO3: check the types for these arguments.
void poisonIfNeeded_getelementptr( 
    Value& dest, APInt& lhs, APInt& rhs, bool inbounds );

void printIfPoison( Instruction& In, APInt& val );
// TODO: see if const qualifiers can be applied to I or val

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_div()
// ----------------------------------------------------------------------------
/*** \brief propogates poison for a division instruction (udiv or
 * sdiv).  Effectively, the instruction's "exact" flag is assumed to
 * be not present, so no new poison can be created.
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param  dest (output) the quotient to check
 * \param  lhs, rhs (inputs) the two operands to check
 *    
 * \return void
 *
 */
inline void poisonIfNeeded_div( APInt& dest, APInt& lhs, APInt& rhs )  
{{
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}


} // End of APIntPoison namespace

} // End of llvm namespace

#endif
