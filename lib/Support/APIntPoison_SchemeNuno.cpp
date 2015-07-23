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

#include <iostream>
#include <stdlib.h>

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

// TODO: adapt this to implement the scheme Nuno and John and David
// and I are fiddling with.

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_add()
// ----------------------------------------------------------------------------
/*** If one of the nsw/nuw is false, no poison can be generated, so no
 *	checking is performed for that kind of wrap.  Of course, if
 *	the result was already poisoned (probably because one of the
 *	operands was poisoned), that poison remains.
 */
void poisonIfNeeded_add_SchemeNuno( APInt& dest, APInt& lhs, APInt& rhs, 
				   bool nsw, bool nuw )
{{
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
  if ( lhs.getPoisoned() || rhs.getPoisoned() )  {
    dest.setPoisoned( true );
    return;
  }
  
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
void poisonIfNeeded_sub_SchemeNuno( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw )
{{
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
  if ( lhs.getPoisoned() || rhs.getPoisoned() )  {
    dest.setPoisoned( true );
    return;
  }
  
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
void poisonIfNeeded_mul_SchemeNuno( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool nsw, bool nuw )
{{
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
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
 */
void poisonIfNeeded_div_SchemeNuno( APInt& dest, APInt& lhs, APInt& rhs, 
			 bool exact )
{{
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
  if ( lhs.getPoisoned() || rhs.getPoisoned() )  {
    dest.setPoisoned( true );
    return;
  }
  
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
void poisonIfNeeded_div_SchemeNuno( APInt& dest, APInt& lhs, APInt& rhs )  
{{
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_rem()
// ----------------------------------------------------------------------------
void poisonIfNeeded_rem_SchemeNuno( APInt& dest, APInt& lhs, APInt& rhs )
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
void poisonIfNeeded_bitAnd_SchemeNuno( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  assert( 
	 ( lhs.getBitWidth() == rhs.getBitWidth() ) && 
	 "Both arguments must have the same bitwidth" );

  if ( lhs.getBitWidth() == 1 )  {
    dest.setPoisoned( 
		     ((lhs == 1) && rhs.getPoisoned()) || 
		     (lhs.getPoisoned() && (rhs == 1))
		      );
    return;
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
void poisonIfNeeded_bitOr_SchemeNuno( APInt& dest, const APInt& lhs, const APInt& rhs )
{{
  assert( 
	 ( lhs.getBitWidth() == rhs.getBitWidth() ) && 
	 "Both arguments must have the same bitwidth" );

  if ( lhs.getBitWidth() == 1 )  {
    dest.setPoisoned( 
		     ((lhs == 1) && rhs.getPoisoned()) || 
		     (lhs.getPoisoned() && (rhs == 1))
		      );
    return;
  }

  // use the classical definition of poison
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_bitXor()
// ----------------------------------------------------------------------------
void poisonIfNeeded_bitXor_SchemeNuno( APInt& dest, const APInt& lhs, const APInt& rhs )
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
void poisonIfNeeded_shl_SchemeNuno( APInt& dest, APInt& src, unsigned shiftAmt,
			 bool nsw, bool nuw )
{{
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
  if ( src.getPoisoned() )  {
    dest.setPoisoned( true );
    return;
  }
  if ( shiftAmt >= src.getBitWidth() )  {
    dest.setPoisoned( true );
    return;
  }

  // if nothing was shifted, no poison can be generated.
  if ( shiftAmt == 0 )  { return; }

  if ( nsw )  { 
    if ( dest.isNegative() )  {
      // did any 0 bits get shifted out?
      if ( ! src.getHiBits(shiftAmt).trunc(shiftAmt).isAllOnesValue()  )  {
      	// an unallowed signed wrap happened
      	dest.orPoisoned(true);
      }
    } else {
      // did any 1 bits get shifted out?
      /* CAS TODO3: this trunc(~) call here may be unnecessary, but it
	 is here for now to guarantee accuracy.
       */
      if ( src.getHiBits(shiftAmt).trunc(shiftAmt) != 0 )  {
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
    if ( src.getHiBits(shiftAmt).trunc(shiftAmt) != 0 )  {
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
void poisonIfNeeded_lshr_SchemeNuno( APInt& dest, APInt& src, unsigned shiftAmt,
			  bool exact )
{{
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
  if ( src.getPoisoned() )  {
    dest.setPoisoned( true );
    return;
  }

  if ( shiftAmt >= src.getBitWidth() )  {
    dest.setPoisoned( true );
    return;
  }

  if ( exact )  { 
    // did any 1 bits get shifted out?
    /* CAS TODO3: this trunc(~) call here may be unnecessary, but it
       is here for now to guarantee accuracy.
     */
    if ( shiftAmt != 0 )  {
      if ( src.getLoBits(shiftAmt).trunc(shiftAmt) != 0 )  {
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
void poisonIfNeeded_ashr_SchemeNuno( APInt& dest, APInt& src, unsigned shiftAmt,
			  bool exact )
{{
  /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     quick check for special cases that propogate or clear poison
   */
  if ( src.getPoisoned() )  {
    dest.setPoisoned( true );
    return;
  }

  if ( shiftAmt >= src.getBitWidth() )  {
    dest.setPoisoned( true );
    return;
  }

  if ( exact )  { 
    // did any 1 bits get shifted out?
    /* CAS TODO3: this trunc(~) call here may be unnecessary, but it
       is here for now to guarantee accuracy.
     */
    if ( shiftAmt != 0 )  {
      if ( src.getLoBits(shiftAmt).trunc(shiftAmt) != 0 )  {
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
void poisonIfNeeded_select_SchemeNuno( APInt& dest,
    const APInt& src1, const APInt& src2, const APInt& src3 )
{{
  /* only propagate poison iff:
      src1 is poisoned
      or
      the selected element of {src2, src3} is poisoned.
    */
  dest.setPoisoned( src1.getPoisoned() );
  dest.orPoisoned( 
      (src1 == 0) ? 
	src3.getPoisoned() : src2.getPoisoned() 
      );
  #if 0 
    std::cerr << "in select opcode: \n";
    if ( src1.getPoisoned() || src2.getPoisoned() || 
	  src3.getPoisoned() || dest.getPoisoned() )  {
       std::cout << "   select: poison bits " << 
	   "src1=" << src1.getPoisoned() <<
	   " src2=" << src2.getPoisoned() <<
	   " src3=" << src3.getPoisoned() <<
	   " dest=" << dest.getPoisoned() << "\n";
    }
    fflush( stdout );
    fflush( stderr );
  #endif

  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_trunc()
// ----------------------------------------------------------------------------
void poisonIfNeeded_trunc_SchemeNuno( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  dest.setPoisoned( src.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_sext()
// ----------------------------------------------------------------------------
void poisonIfNeeded_sext_SchemeNuno( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  dest.setPoisoned( src.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_zext()
// ----------------------------------------------------------------------------
void poisonIfNeeded_zext_SchemeNuno( APInt& dest, const APInt& src, 
    const unsigned newBitWidth )
{{
  dest.setPoisoned( src.getPoisoned() );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_ptrtoint()
// ----------------------------------------------------------------------------
void poisonIfNeeded_ptrtoint_SchemeNuno( APInt& dest, const APInt& src ) 
{{
  // TODO2: src needs to be of a pointer type.
  // TODO2: update this once we find a way to represent poison in pointers.
  dest.setPoisoned( false );
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_inttoptr()
// ----------------------------------------------------------------------------
void poisonIfNeeded_inttoptr_SchemeNuno( APInt& dest, const APInt& src )
{{
  // TODO2: dest needs to be of a pointer type.
  // TODO2: update this once we find a way to represent poison in pointers.
  if ( src.getPoisoned() )  {
    std::cout << "Converting a poisoned int to a pointer is unsupported.\n";
    exit( 1 );
  }
  return;
}}

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_bitcast()
// ----------------------------------------------------------------------------
void poisonIfNeeded_bitcast_SchemeNuno( APInt& dest, const APInt& src ) 
{{
  dest.setPoisoned( src.getPoisoned() );
  return;
}}
	
// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_icmp()
// ----------------------------------------------------------------------------
void poisonIfNeeded_icmp_SchemeNuno( APInt& dest, 
				     const APInt& lhs, const APInt& rhs ) 
{{
  dest.setPoisoned( lhs.getPoisoned() || rhs.getPoisoned() );
  return;
}}
	
// ----------------------------------------------------------------------------
///  Note: phi and call are handled by normal poison propogation in APInt
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_br()
// ----------------------------------------------------------------------------
void poisonIfNeeded_br_SchemeNuno()
{{
  // TODO: find some way to pass in the value of the br arguments.
  // Then update this function per the nominal "Nuno" document.
  return;
}}
	
// ----------------------------------------------------------------------------
/// \brief same interface as poisonIfNeeded_getelementptr()
// ----------------------------------------------------------------------------
void poisonIfNeeded_getelementptr_SchemeNuno( Value& dest, 
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

