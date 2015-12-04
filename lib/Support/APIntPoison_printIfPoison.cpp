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
/// \fn function printIfPoison()
/** ---------------------------------------------------------------------------
 * \brief: prints to stdout the source file and line number if the given
 *	APInt is poisoned.  Only prints if option opt_print_new_poison is set.
 *
 * \b Detailed_Description: 
 *
 * \b Method: 
 *
 * \b Reentrancy: 
 *
 * \param In (input) the Instruction where the poison was generated
 * \param val (input) the APInt to check
 *    
 * \return void
 *
 */
void printIfPoison( Instruction& In, APInt& val )
{{

  if ( llvm::lli_undef_fix::opt_poison_eq_undef_early ) {  
    val.setRandom();
    val.setPoisoned(false);
  }

  if ( ! llvm::lli_undef_fix::opt_print_new_poison ) { return; }
  static raw_fd_ostream stream( 2, false, true );

  if ( ! val.getPoisoned() )  { return; }

  // TODO: get this to link and remove the #if.
  DebugLoc dl= In.getDebugLoc();
  stream << "poison found at: line " << dl.getLine() << ": " << 
      In.getOpcodeName() << " \n";
  dl.print( stream );
  /* TODO2: figure out why dl.print(~) prints nothing for .ll files--
	 is it that there is no C source debug info in the .ll files we're
	 using?
   */
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

