/*** 
   * Program Name: lli_undef_fix
   *
   * \file LLIUndefFix.cpp
   *
   * \brief
   *
   * \b Detailed_Description:
   *
   * <$project_name) was written by Christian A. Schreiner at University of
   * Utah.  Copyright (C) 2015-2015 by University of Utah.  All rights
   * reserved.  You may use, examine, or modify this file only in accordance
   * with the GNU Public License, or, alternately, by special written
   * arrangement with the author.  This file comes with no warranties.  If you
   * use it and something breaks, you are solely responsible for cleaning up
   * afterwards.
   */

/*++ Configuration Management Information:
   * 
   * $Source: /net/home/cas/templates/auto_uut/RCS/cas_tmpl.cpp,v $
   * $File: $
   * $Author: cas $
   * $Date: 2015/02/24 07:14:20 $
   * $Revision: 1.1 $
   * 
   */

/*++ **************************************************************************
   *   includes
   * **************************************************************************
   */

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/LLIUndefFix.h"

namespace llvm { 
namespace lli_undef_fix { 

/*++ **************************************************************************
   *   declarations
   * **************************************************************************
   */

/** \brief Holds value of the '--luf-antidote-select' command line option.
	This makes a SELECT instruction ignore poison from the unselected
	operand.  I.e. the return value is only poisoned if the condition is
	poisoned, or if the chosen operand is poisoned.
*/
llvm::cl::opt<bool> arg_luf_antidote_select( 
    "luf-antidote-select",
    cl::desc("make SELECT ignore poison from the unselected operand"),   
    cl::init(false) );



/*++ ==========================================================================
   *   un-submoduled function prototypes
   * ==========================================================================
   */

/*++ **************************************************************************
   *   source code
   * **************************************************************************
   */

// template is 20 lines long
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
   * Outputs: 
   * \param yy (output)
   *
   * \return 
   *
   */
//void name()


/*++ **************************************************************************
   *   close namespaces
   * **************************************************************************
   */
} // namespace lli_undef_fix
} // namespace llvm

/*++ **************************************************************************
   *   end of file
   * **************************************************************************
   */

