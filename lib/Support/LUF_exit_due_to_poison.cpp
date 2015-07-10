/*** 
   * Program Name: lli_undef_fix 
   *
   * \file LUF_exit_due_to_poison.cpp 
   *
   * \brief implements the exit_due_to_poison() function
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
   * $Date: 2015/02/28 04:15:43 $
   * $Revision: 1.4 $
   * 
   */

/*++ **************************************************************************
   *   includes
   * **************************************************************************
   */
#include <iostream>
#include <stdlib.h>

#include "llvm/Support/LUF_etc.h"

/*++ **************************************************************************
   *   declarations
   * **************************************************************************
   */

/*++ ==========================================================================
   *   un-submoduled function prototypes
   * ==========================================================================
   */

/*++ **************************************************************************
   *   source code
   * **************************************************************************
   */

// ----------------------------------------------------------------------------
///  \fn exit_due_to_poison()
// ----------------------------------------------------------------------------
/*** \brief exits with an appropriate error message about poison
   *
   * \b Detailed_Description: 
   *
   * \b Method: 
   *
   * \b Reentrancy: 
   *
   * \return void
   *
   */
void llvm::lli_undef_fix::exit_due_to_poison()
{{
  fflush ( stdout );
  fflush ( stderr );
  std::cerr << "exiting due to external poison propogation. \n";
  exit( EXIT_FAILURE );
}}


// template is 22 lines long
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


/*++ **************************************************************************
   *   closing
   * **************************************************************************
   */

/*++ **************************************************************************
   *   end of file
   * **************************************************************************
   */

