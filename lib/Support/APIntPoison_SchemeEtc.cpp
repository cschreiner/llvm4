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

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_add()
// ----------------------------------------------------------------------------
/*** If one of the nsw/nuw is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 */
void poisonIfNeeded_add_SchemeEtc( APInt& dest, APInt& lhs, APInt& rhs, 
				   bool nsw, bool nuw )
{{
  if ( nsw )  { 
    if ( rhs.slt(0) ? dest.sgt(lhs) : dest.slt(lhs) )  {
      // an unallowed signed wrap happened
      dest.orPoisoned(true);
    }
  }
  if ( nuw )  { 
    if ( dest.ult(lhs) || dest.ult(rhs) )  {
      // an unallowed unsigned wrap happened
      dest.orPoisoned(true);
    }
  }
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_sub()
// ----------------------------------------------------------------------------
/*** If one of the nsw/nuw is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 */
void poisonIfNeeded_sub_SchemeEtc( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw )
{{
  if ( nsw )  { 
    if ( rhs.sgt(0) ? dest.sgt(lhs) : dest.slt(lhs) )  {
      // an unallowed signed wrap happened
      dest.orPoisoned(true);
    }
  }
  if ( nuw )  { 
    if ( lhs.ult(rhs) )  {
      // an unallowed unsigned wrap happened
      dest.orPoisoned(true);
    }
  }
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_mul()
// ----------------------------------------------------------------------------
/***	If one of the nsw/nuw is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 */
void poisonIfNeeded_mul_SchemeEtc( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw )
{{

  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
  if ( llvm::lli_undef_fix::opt_antidote_and_or )  {
    if ( lhs.getPoisoned() && (!rhs) )  { 
      // poison times zero is unpoisoned zero
      dest.setPoisoned( false );
      return;
    }
    if ( (!lhs) && rhs.getPoisoned() )  { 
      // zero times poison is unpoisoned zero
      dest.setPoisoned( false );
      return;
    }
  }
  if ( lhs.getPoisoned() || rhs.getPoisoned() )  {
    dest.setPoisoned( true );
    return;
  }
  
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     determine if new poison was created by forbidden signed wraparound
   */
  if ( nsw )  { 
    // algorithm from:
    // https://www.securecoding.cert.org/confluence/display/seccode/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow

    APInt int_max= APInt::getSignedMaxValue( dest.getBitWidth() );
    APInt int_min= APInt::getSignedMinValue( dest.getBitWidth() );
    if ( lhs.sgt(0) ) {  /* lhs is positive */
      if (rhs.sgt(0) > 0) {  /* lhs and rhs are positive */
	if (lhs.sgt( (int_max.sdiv(rhs) )) )  {
	  // an unallowed signed wrap happened
	  dest.orPoisoned(true);
	}
      } else { /* lhs positive, rhs nonpositive */
	if (rhs.slt( (int_min.sdiv(lhs) )) )  {
	  // an unallowed signed wrap happened
	  dest.orPoisoned(true);
	}
      } /* lhs positive, rhs nonpositive */
    } else { /* lhs is nonpositive */
      if (rhs.sgt(0) )  { /* lhs is nonpositive, rhs is positive */
	if (lhs.slt( (int_min.sdiv(rhs) )) )  {
	  // an unallowed signed wrap happened
	  dest.orPoisoned(true);
	}
      } else { /* lhs and rhs are nonpositive */
	if ( (lhs != 0) && (rhs.slt( (int_max.sdiv(lhs) )) ) )  {
	  // an unallowed signed wrap happened
	  dest.orPoisoned(true);
	}
      } /* End if lhs and rhs are nonpositive */
    } /* End if lhs is nonpositive */
  }

  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     determine if new poison was created by forbidden unsigned wraparound
   */
  if ( nuw )  { 
    // algorithm from: 
    // http://stackoverflow.com/questions/199333/best-way-to-detect-integer-overflow-in-c-c
    unsigned lhs_digits= lhs.getBitWidth()- lhs.countLeadingZeros();
    unsigned rhs_digits= rhs.getBitWidth()- rhs.countLeadingZeros();
    if ( (lhs_digits + rhs_digits) > dest.getBitWidth() )  {
      // an unallowed unsigned wrap happened
      dest.orPoisoned(true);
    }
  }

  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     clean up and return
   */
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_div()
// ----------------------------------------------------------------------------
/***	If the exact flag is false, no poison can be generated, so no
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
void poisonIfNeeded_div_SchemeEtc( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool exact )
{{
  if ( exact )  { 
    if ( (rhs * dest) != lhs )  {
      // an unallowed remainder happened
      dest.orPoisoned(true);
    }
  }
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_div( APInt&, APInt&, ApInt& )
// ----------------------------------------------------------------------------
void poisonIfNeeded_div_SchemeEtc( APInt& dest, APInt& lhs, APInt& rhs )  
{{
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_rem()
// ----------------------------------------------------------------------------
void poisonIfNeeded_rem_SchemeEtc( APInt& dest, APInt& lhs, APInt& rhs )
{{
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_bitAnd()
// ----------------------------------------------------------------------------
/*** \b Method: If option opt_antidote_and_or is set, uses
   *	short-circuit poison propogation (i.e. 0 and poison= unpoisoned
   *	0).  Otherwise propogates poison if either operand is
   *	poisoned.
   */
void poisonIfNeeded_bitAnd_SchemeEtc( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  if ( llvm::lli_undef_fix::opt_antidote_and_or || 
      llvm::lli_undef_fix::opt_poison_eq_undef )  {
    if ( lhs.getPoisoned() && rhs.getPoisoned() )  {
       dest.setPoisoned( true );
       return;
    }
    if ( (!lhs.getPoisoned()) && (!rhs.getPoisoned()) )  {
       dest.setPoisoned( false );
       return;
    }
    // check for lhs is poisoned, rhs is unpoisoned and zero.
    if ( lhs.getPoisoned() && (!rhs.getPoisoned()) && (!rhs) )  {
       // a corrupted value in lhs does not affect the result.
       dest.setPoisoned( false );
       return;
    }
    // check for lhs is unpoisoned and zero, rhs is poisoned.
    if ( (!lhs.getPoisoned()) && (!lhs) && rhs.getPoisoned() )  {
       // a corrupted value in rhs does not affect the result.
       dest.setPoisoned( false );
       return;
    }
  } 

  // use the classical definition of poison
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_bitOr()
// ----------------------------------------------------------------------------
/*** \b Method: If option opt_antidote_and_or is set, uses
   *	short-circuit poison propogation (i.e. 1 or poison= unpoisoned
   *	1).  Otherwise propogates poison if either operand is
   *	poisoned.
   */
void poisonIfNeeded_bitOr_SchemeEtc( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  if ( llvm::lli_undef_fix::opt_antidote_and_or ||
      llvm::lli_undef_fix::opt_poison_eq_undef )  {
    if ( lhs.getPoisoned() && rhs.getPoisoned() )  {
       dest.setPoisoned( true );
       return;
    }
    if ( (!lhs.getPoisoned()) && (!rhs.getPoisoned()) )  {
       dest.setPoisoned( false );
       return;
    }
    // check for lhs is poisoned, rhs is unpoisoned and one.
    if ( lhs.getPoisoned() && (!rhs.getPoisoned()) && rhs.isAllOnesValue() )  {
       // a corrupted value in lhs does not affect the result.
       dest.setPoisoned( false );
       return;
    }
    // check for lhs is unpoisoned and zero, rhs is poisoned.
    if ( (!lhs.getPoisoned()) && lhs.isAllOnesValue() && rhs.getPoisoned() )  {
       // a corrupted value in rhs does not affect the result.
       dest.setPoisoned( false );
       return;
    }
  } 

  // use the classical definition of poison
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_bitXor()
// ----------------------------------------------------------------------------
void poisonIfNeeded_bitXor_SchemeEtc( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_shl()
// ----------------------------------------------------------------------------
/*** Note: if signed and/or unsigned overshift is forbidden, mark the
 *	destination as poisoned if the given shl operands would create a
 *	poison value.
 * Note: "Wrap" for shifting purposes means "overshift". If one of the
 *	nsw/nuw flags is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 */
void poisonIfNeeded_shl_SchemeEtc( APInt& dest, APInt& lhs, unsigned shiftAmt,
			 bool nsw, bool nuw )
{{
  // if nothing was shifted, no poison can be generated.
  if ( shiftAmt == 0 )  { return; }

  if ( nsw )  { 
    if ( dest.isNegative() )  {
      // did any 0 bits get shifted out?
      if ( ! lhs.getHiBits(shiftAmt).trunc(shiftAmt).isAllOnesValue()  )  {
      	// an unallowed signed wrap happened
      	dest.orPoisoned(true);
      }
    } else {
      // did any 1 bits get shifted out?
      /* CAS TODO3: this trunc(~) call here may be unnecessary, but it
	 is here for now to guarantee accuracy.
       */
      if ( lhs.getHiBits(shiftAmt).trunc(shiftAmt) != 0 )  {
      	// an unallowed signed wrap happened
      	dest.orPoisoned(true);
      }
    }
  }
  if ( nuw )  { 
    // did any 1 bits get shifted out?
    /* CAS TODO3: this trunc(~) call here may be unnecessary, but it
       is here for now to guarantee accuracy.
     */
    if ( lhs.getHiBits(shiftAmt).trunc(shiftAmt) != 0 )  {
      // an unallowed unsigned wrap happened
      dest.orPoisoned(true);
    }
  }
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_lshr()
// ----------------------------------------------------------------------------
/***	If the exact flag is false, no poison can be generated, so no
 *	checking is performed.  Of course, if the result was already
 *	poisoned (probably because one of the operands was poisoned),
 *	that poison remains.
 */
void poisonIfNeeded_lshr_SchemeEtc( APInt& dest, APInt& lhs, unsigned shiftAmt,
			  bool exact )
{{
  if ( exact )  { 
    // did any 1 bits get shifted out?
    /* CAS TODO3: this trunc(~) call here may be unnecessary, but it
       is here for now to guarantee accuracy.
     */
    if ( shiftAmt != 0 )  {
      if ( lhs.getLoBits(shiftAmt).trunc(shiftAmt) != 0 )  {
	// an unallowed unsigned wrap happened
	dest.orPoisoned(true);
      }
    }
  }
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_ashr()
// ----------------------------------------------------------------------------
/***	If the exact flag is false, no poison can be generated, so no
 *	checking is performed.  Of course, if the result was already
 *	poisoned (probably because one of the operands was poisoned),
 *	that poison remains.
 */
void poisonIfNeeded_ashr_SchemeEtc( APInt& dest, APInt& lhs, unsigned shiftAmt,
			  bool exact )
{{
  if ( exact )  { 
    // did any 1 bits get shifted out?
    /* CAS TODO3: this trunc(~) call here may be unnecessary, but it
       is here for now to guarantee accuracy.
     */
    if ( shiftAmt != 0 )  {
      if ( lhs.getLoBits(shiftAmt).trunc(shiftAmt) != 0 )  {
	// an unallowed unsigned wrap happened
	dest.orPoisoned(true);
      }
    }
  }
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_select()
// ----------------------------------------------------------------------------
void poisonIfNeeded_select_SchemeEtc( APInt& Dest,
    const APInt& Src1, const APInt& Src2, const APInt& Src3 )
{{
  /* TODO: get rid of this, and the #include <stdlib.h> above, if we can use
      the opt_select_antidote variable. 
  */

  if ( lli_undef_fix::opt_antidote_select )  { 
    /* this is the default behavior */
    /* CAS TODO: make the above if be dependant on a command-line parameter */
    Dest.setPoisoned( Src1.getPoisoned() );
    Dest.orPoisoned( Src2, Src3 );
  } else {
    /* only propagate poison iff:
	Src1 is poisoned
	or
	the selected element of {Src2, Src3} is poisoned.
      */
    Dest.setPoisoned( Src1.getPoisoned() );
    Dest.orPoisoned( 
	(Src1 == 0) ? 
	  Src3.getPoisoned() : Src2.getPoisoned() 
	);
  }
  #if 0 
    std::cerr << "in select opcode: \n";
    if ( Src1.getPoisoned() || Src2.getPoisoned() || 
	  Src3.getPoisoned() || Dest.getPoisoned() )  {
       std::cout << "   select: poison bits " << 
	   "Src1=" << Src1.getPoisoned() <<
	   " Src2=" << Src2.getPoisoned() <<
	   " Src3=" << Src3.getPoisoned() <<
	   " Dest=" << Dest.getPoisoned() << "\n";
    }
    fflush( stdout );
    fflush( stderr );
  #endif

  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_trunc()
// ----------------------------------------------------------------------------
void poisonIfNeeded_trunc_SchemeEtc( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  dest.setPoisoned( src.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_sext()
// ----------------------------------------------------------------------------
void poisonIfNeeded_sext_SchemeEtc( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  dest.setPoisoned( src.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_zext()
// ----------------------------------------------------------------------------
void poisonIfNeeded_zext_SchemeEtc( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  dest.setPoisoned( src.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_ptrtoint()
// ----------------------------------------------------------------------------
void poisonIfNeeded_ptrtoint_SchemeEtc( APInt& dest, const APInt& src ) 
{{
  // intentionally nothing
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_inttoptr()
// ----------------------------------------------------------------------------
void poisonIfNeeded_inttoptr_SchemeEtc( APInt& dest, const APInt& src )
{{
  // intentionally nothing
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_bitcast()
// ----------------------------------------------------------------------------
void poisonIfNeeded_bitcast_SchemeEtc( APInt& dest, const APInt& src ) 
{{
  // intentionally nothing
  return;
}}
	
// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_icmp()
// ----------------------------------------------------------------------------
void poisonIfNeeded_icmp_SchemeEtc( APInt& dest, 
				     const APInt& lhs, const APInt& rhs ) 
{{
  // intentionally nothing
  return;
}}
	
// ----------------------------------------------------------------------------
///  Note: phi and call are handled by normal poison propogation in APInt
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_br()
// ----------------------------------------------------------------------------
void poisonIfNeeded_br_SchemeEtc()
{{
  // intentionally nothing
  return;
}}
	
// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_getelementptr()
// ----------------------------------------------------------------------------
void poisonIfNeeded_getelementptr_SchemeEtc( Value& dest, 
    const APInt& lhs, const APInt& rhs,
    bool inbounds )
{{
  // intentionally nothing
  return;
}}
	

} // end namespace APIntPoison
// ############################################################################


} // end namespace llvm
// ############################################################################

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

