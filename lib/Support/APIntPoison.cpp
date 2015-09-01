//===-- APInt.cpp - Implement APInt class ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements functions to help class APInt track poison
// propogation through its arithmetic operations.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/DebugLoc.h"

#include "llvm/ADT/APInt.h"
#include "llvm/ADT/APIntPoison.h"

#include "llvm/Support/LUF_opts.h"

using namespace llvm;

// ############################################################################
namespace llvm {

// ############################################################################
namespace APIntPoison {

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_add()
 * ----------------------------------------------------------------------------
 * Description: if signed and/or unsigned wraparound is forbidden, mark the
 *	destination as poisoned if the given add operands would create a
 *	poison value.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the sum to check
 *   lhs, rhs: the two operands to check
 *   nsw, nuw: true if the "no signed wrap" (nsw) or "no unsigned wrap" (nuw) 
 *	flag was present on the LLVM instruction.
 *	If one of these is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 *    
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_add( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_add_SchemeNuno( dest, lhs, rhs, nsw, nuw );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_add_SchemeNunoPP( dest, lhs, rhs, nsw, nuw );
  }
  return poisonIfNeeded_add_SchemeEtc( dest, lhs, rhs, nsw, nuw );
}}

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_sub()
 * ----------------------------------------------------------------------------
 * Description: if signed and/or unsigned wraparound is forbidden, mark the
 *	destination as poisoned if the given sub operands would create a
 *	poison value.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the difference to check
 *   lhs, rhs: the two operands to check
 *   nsw, nuw: true if the "no signed wrap" (nsw) or "no unsigned wrap" (nuw) 
 *	flag was present on the LLVM instruction.
 *	If one of these is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 *    
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_sub( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_sub_SchemeNuno( dest, lhs, rhs, nsw, nuw );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_sub_SchemeNunoPP( dest, lhs, rhs, nsw, nuw );
  }
  return poisonIfNeeded_sub_SchemeEtc( dest, lhs, rhs, nsw, nuw );
}}

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_mul()
 * ----------------------------------------------------------------------------
 * Description: if signed and/or unsigned wraparound is forbidden, mark the
 *	destination as poisoned if the given mul operands would create a
 *	poison value.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the product to check
 *   lhs, rhs: the two operands to check
 *   nsw, nuw: true if the "no signed wrap" (nsw) or "no unsigned wrap" (nuw) 
 *	flag was present on the LLVM instruction.
 *	If one of these is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 *    
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_mul( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_mul_SchemeNuno( dest, lhs, rhs, nsw, nuw );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_mul_SchemeNunoPP( dest, lhs, rhs, nsw, nuw );
  }
  return poisonIfNeeded_mul_SchemeEtc( dest, lhs, rhs, nsw, nuw );
}}

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_div()
 * ----------------------------------------------------------------------------
 *
 * Description: if a remainder is forbidden, mark the destination as poisoned
 *	if the given div operands would create a poison value.
 *	This works for both the sdiv and udiv instructions.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the quotient to check
 *   lhs, rhs: the two operands to check
 *   exact: true if the "exact" flag was present on the LLVM instruction.
 *	If it is false, no poison can be generated, so no
 *	checking is performed for remainders.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 *    
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_div( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool exact )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_div_SchemeNuno( dest, lhs, rhs, exact );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_div_SchemeNunoPP( dest, lhs, rhs, exact );
  }
  return poisonIfNeeded_div_SchemeEtc( dest, lhs, rhs, exact );
}}

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
void poisonIfNeeded_div( APInt& dest, APInt& lhs, APInt& rhs )  
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_div_SchemeNuno( dest, lhs, rhs );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_div_SchemeNunoPP( dest, lhs, rhs );
  }
  return poisonIfNeeded_div_SchemeEtc( dest, lhs, rhs );
}}

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_rem()
 * ----------------------------------------------------------------------------
 *
 * Description: propogates poison for a remainder (srem or urem) instruction.
 *	Note that neither remainder instruction can create new poison.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the quotient to check
 *   lhs, rhs: the two operands to check
 *    
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_rem( APInt& dest, APInt& lhs, APInt& rhs )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_rem_SchemeNuno( dest, lhs, rhs );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_rem_SchemeNunoPP( dest, lhs, rhs );
  }
  return poisonIfNeeded_rem_SchemeEtc( dest, lhs, rhs );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_bitAnd()
// ----------------------------------------------------------------------------
/*** \brief determines if the result of a "bitwise and" operation is poisoned
   *
   * \b Detailed_Description: 
   *
   * \b Method: If option opt_antidote_and_or is set, uses
   *	short-circuit poison propogation (i.e. 0 and poison= unpoisoned
   *	0).  Otherwise propogates poison if either operand is
   *	poisoned.
   *
   * \b Reentrancy: 
   *
   * \param lhs, rhs (input) the left and right operands
   *    
   * \param dest (output) the result of the AND operation
   *
   * \return void
   *
   */
void poisonIfNeeded_bitAnd( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_bitAnd_SchemeNuno( dest, lhs, rhs );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_bitAnd_SchemeNunoPP( dest, lhs, rhs );
  }
  return poisonIfNeeded_bitAnd_SchemeEtc( dest, lhs, rhs );
}}


// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_bitOr()
// ----------------------------------------------------------------------------
/*** \brief determines if the result of a "bitwise or" operation is poisoned
   *
   * \b Detailed_Description: 
   *
   * \b Method: If option opt_antidote_and_or is set, uses
   *	short-circuit poison propogation (i.e. 1 or poison= unpoisoned
   *	1).  Otherwise propogates poison if either operand is
   *	poisoned.
   *
   * \b Reentrancy: 
   *
   * \param lhs, rhs (input) the left and right operands
   *    
   * \param dest (output) the result of the AND operation
   *
   * \return void
   *
   */
void poisonIfNeeded_bitOr( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_bitOr_SchemeNuno( dest, lhs, rhs );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_bitOr_SchemeNunoPP( dest, lhs, rhs );
  }
  return poisonIfNeeded_bitOr_SchemeEtc( dest, lhs, rhs );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_bitXor()
// ----------------------------------------------------------------------------
/*** \brief determines if the result of a bitwise XOR instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param dest (output) write the result here
 *    
 * \param lhs, rhs (intputs) the two sides of the XOR
 *
 * \return void
 *
 */
void poisonIfNeeded_bitXor( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_bitXor_SchemeNuno( dest, lhs, rhs );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_bitXor_SchemeNunoPP( dest, lhs, rhs );
  }
  return poisonIfNeeded_bitXor_SchemeEtc( dest, lhs, rhs );
}}

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_shl()
 * ----------------------------------------------------------------------------
 * Description: if signed and/or unsigned overshift is forbidden, mark the
 *	destination as poisoned if the given shl operands would create a
 *	poison value.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the result to check
 *   lhs: the APInt that got shifted
 *   shiftAmt: the number of places to shift left
 *   nsw, nuw: true if the "no signed wrap" (nsw) or "no unsigned wrap" (nuw) 
 *	flag was present on the LLVM instruction.  "Wrap" for shifting 
 *	purposes means "overshift". If one of these is false, no poison can be
 *	generated, so no checking is performed for that kind of wrap.  Of
 *	course, if the result was already poisoned (probably because one of
 *	the operands was poisoned), that poison remains.
 *    
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_shl( APInt& dest, APInt& src, unsigned shiftAmt,
			 bool nsw, bool nuw )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_shl_SchemeNuno( dest, src, shiftAmt, nsw, nuw );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_shl_SchemeNunoPP( dest, src, shiftAmt, nsw, nuw );
  }
  return poisonIfNeeded_shl_SchemeEtc( dest, src, shiftAmt, nsw, nuw );
}}

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_lshr()
 * ----------------------------------------------------------------------------
 * Description: if an exact shift is required, mark the destination as
 *	poisoned if the given lshr operands would create a poison value.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the result to check
 *   lhs: the APInt that got shifted
 *   shiftAmt: the number of places to shift left
 *   exact: true if the "exact" flag was present on the LLVM instruction.  
 *	If this is false, no poison can be generated, so no checking is
 *	performed.  Of course, if the result was already poisoned (probably
 *	because one of the operands was poisoned), that poison remains.
 *     
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_lshr( APInt& dest, APInt& src, unsigned shiftAmt,
			  bool exact )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_lshr_SchemeNuno( dest, src, shiftAmt, exact );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_lshr_SchemeNunoPP( dest, src, shiftAmt, exact );
  }
  return poisonIfNeeded_lshr_SchemeEtc( dest, src, shiftAmt, exact );
}}

/*** --------------------------------------------------------------------------
 * function poisonIfNeeded_ashr()
 * ----------------------------------------------------------------------------
 * Description: if an inbounds result is required, mark the destination as
 *	poisoned if the given getelementptr operands would create a poison 
 *	value.
 *
 * Method: 
 *
 * Reentrancy: 
 *
 * Inputs: 
 *   dest: the result to check
 *   src: the APInt that got shifted
 *   shiftAmt: the number of places to shift left
 *   exact: true if the "exact" flag was present on the LLVM instruction.  
 *	If this is false, no poison can be generated, so no checking is
 *	performed.  Of course, if the result was already poisoned (probably
 *	because one of the operands was poisoned), that poison remains.
 *     
 * Outputs: 
 *   dest: write the poison result here
 *
 * Return Value: none
 *
 */
void poisonIfNeeded_ashr( APInt& dest, APInt& src, unsigned shiftAmt,
			  bool exact )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_ashr_SchemeNuno( dest, src, shiftAmt, exact );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_ashr_SchemeNunoPP( dest, src, shiftAmt, exact );
  }
  return poisonIfNeeded_ashr_SchemeEtc( dest, src, shiftAmt, exact );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_select()
// ----------------------------------------------------------------------------
/*** \brief determines if the result of a select instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param dest (output) write the result here
 *    
 * \param src1, src2, src3 (intputs) the parameters of the select
 *
 * \return void
 *
 */
void poisonIfNeeded_select( APInt& dest,
    const APInt& src1, const APInt& src2, const APInt& src3 )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_select_SchemeNuno( dest, src1, src2, src3 );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_select_SchemeNunoPP( dest, src1, src2, src3 );
  }
  return poisonIfNeeded_select_SchemeEtc( dest, src1, src2, src3 );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_trunc()
// ----------------------------------------------------------------------------
/*** \brief determines if the result of a trunc instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param dest (output) write the result here
 *    
 * \param src (intput) the value to truncate
 *
 * \param newBitWidth (input) the value's new width
 *
 * \return void
 *
 */
void poisonIfNeeded_trunc( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_trunc_SchemeNuno( dest, src, newBitWidth );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_trunc_SchemeNunoPP( dest, src, newBitWidth );
  }
  return poisonIfNeeded_trunc_SchemeEtc( dest, src, newBitWidth );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_sext()
// ----------------------------------------------------------------------------
/*** \brief determines if the result of a sext instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param dest (output) write the result here
 *    
 * \param src (intput) the value to extend
 *
 * \param newBitWidth (input) the value's new width
 *
 * \return void
 *
 */
void poisonIfNeeded_sext( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_sext_SchemeNuno( dest, src, newBitWidth );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_sext_SchemeNunoPP( dest, src, newBitWidth );
  }
  return poisonIfNeeded_sext_SchemeEtc( dest, src, newBitWidth );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_zext()
// ----------------------------------------------------------------------------
/*** \brief determines if the result of a zext instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param dest (output) write the result here
 *    
 * \param src (intput) the value to extend
 *
 * \param newBitWidth (input) the value's new width
 *
 * \return void
 *
 */
void poisonIfNeeded_zext( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_zext_SchemeNuno( dest, src, newBitWidth );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_zext_SchemeNunoPP( dest, src, newBitWidth );
  }
  return poisonIfNeeded_zext_SchemeEtc( dest, src, newBitWidth );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_ptrtoint()
// ----------------------------------------------------------------------------
/*** \brief determines of the result of a ptrtoint instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param ptrtoint (input) 
 *    
 * \param dest (output) write the result here
 *    
 * \param src (intput) the value to convert
 *
 * \return void
 *
 */
void poisonIfNeeded_ptrtoint( APInt& dest, const APInt& src )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_ptrtoint_SchemeNuno( dest, src );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_ptrtoint_SchemeNunoPP( dest, src );
  }
  return poisonIfNeeded_ptrtoint_SchemeEtc( dest, src );
}}
	
// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_inttoptr()
// ----------------------------------------------------------------------------
/*** \brief determines of the result of an inttoptr instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param inttoptr (input) 
 *    
 * \param dest (output) write the result here
 *    
 * \param src (intput) the value to convert
 *
 * \return void
 *
 */
void poisonIfNeeded_inttoptr( APInt& dest, const APInt& src )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_inttoptr_SchemeNuno( dest, src );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_inttoptr_SchemeNunoPP( dest, src );
  }
  return poisonIfNeeded_inttoptr_SchemeEtc( dest, src );
}}

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_bitcast()
// ----------------------------------------------------------------------------
/*** \brief determines of the result of a bitcast instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param bitcast (input) 
 *    
 * \param dest (output) write the result here
 *    
 * \param src (intput) the value to convert
 *
 * \return void
 *
 */
void poisonIfNeeded_bitcast( APInt& dest, const APInt& src )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_inttoptr_SchemeNuno( dest, src );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_inttoptr_SchemeNunoPP( dest, src );
  }
  return poisonIfNeeded_inttoptr_SchemeEtc( dest, src );
}}
	
// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_icmp()
// ----------------------------------------------------------------------------
/*** \brief determines of the result of an icmp instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param dest (output) write the result here
 *    
 * \param lhs (intput) the left value to compare
 * \param rhs (intput) the right value to compare
 *
 * \return void
 *
 */
void poisonIfNeeded_icmp( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_icmp_SchemeNuno( dest, lhs, rhs );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_icmp_SchemeNunoPP( dest, lhs, rhs );
  }
  return poisonIfNeeded_icmp_SchemeEtc( dest, lhs, rhs );
}}
	
// ----------------------------------------------------------------------------
///  Note: phi and call are handled by normal poison propogation in APInt
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_br()
// ----------------------------------------------------------------------------
/*** \brief determines of the result of a br instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param xx (input) TODO: add parameters once this is thought through
 *    
 * \param yy (output) 
 *
 * \return void
 *
 */
void poisonIfNeeded_br()
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    poisonIfNeeded_br_SchemeNuno(); 
    return;
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    poisonIfNeeded_br_SchemeNunoPP(); 
    return;
  }
  poisonIfNeeded_br_SchemeEtc(); 
  return;
}}
	
// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_getelementptr()
// ----------------------------------------------------------------------------
/*** \brief determines of the result of a br instruction is poisoned
 *
 * \b Detailed_Description: 
 *
 * \b Method: this can probably be implemented as "if the src pointer is 
 *	poisoned, so is the destination."
 *
 * \b Reentrancy: 
 *
 *  CAS TODO3: check the types for these arguments.
 *
 * \param dest (output) write the result here
 *
 * \param lhs (input) the first parameter 
 *    
 * \param rhs (input) the second parameter
 *    
 * \param inbounds (input) true IFF the inbounds flag is present
 *    
 * \return void
 *
 */
 void poisonIfNeeded_getelementptr( Value& dest, 
				    const APInt& lhs, const APInt& rhs, 
				    bool inbounds)
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_getelementptr_SchemeNuno( dest, lhs, rhs, inbounds ); 
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_getelementptr_SchemeNunoPP( dest, lhs, rhs, inbounds ); 
  }
  return poisonIfNeeded_getelementptr_SchemeEtc( dest, lhs, rhs, inbounds ); 
}}
	
} // end poisonIfNeeded_bitcastspace APIntPoison
// ############################################################################


} // end namespace llvm
// ############################################################################

// special template is 27 lines long
#if 0
// ----------------------------------------------------------------------------
///  \fn poisonIfNeeded_xx()
// ----------------------------------------------------------------------------
// CAS TODO: implement this sometime
/*** \brief CAS TODO: implement this sometime
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param xx (input) 
 *    
 * \param yy (output) 
 *
 * \return void
 *
 */
void poisonIfNeeded_xx()
{{
  if ( llvm::lli_undef_fix::opt_nuno )  {
    return poisonIfNeeded_xx_SchemeNuno( dest, src );
  }
  if ( llvm::lli_undef_fix::opt_nuno_pp )  {
    return poisonIfNeeded_xx_SchemeNunoPP( dest, src );
  }
  return poisonIfNeeded_xx_SchemeEtc( dest, src );
}}

#endif // end special template

// template is 21 lines long
// ----------------------------------------------------------------------------
///  \fn name()
// ----------------------------------------------------------------------------
/*** \brief 
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param xx (input) 
 *    
 * \param yy (output) 
 *
 * \return 
 *
 */
//void name()
//{{
//}}
	
// CAS TODO2: figure out what this DEBUG_TYPE is for -- 2015mar14
#define DEBUG_TYPE "apint"

