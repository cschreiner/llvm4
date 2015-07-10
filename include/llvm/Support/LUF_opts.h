/*** 
   * Program Name: lli_undef_fix
   *
   * File Name: LUF_opts.h
   *
   * File Description: handle options for LLI Undef Fix (LUF) additions to llvm.
   *
   * lli_undef_fix was written by Christian A. Schreiner at University of
   * Utah.  Copyright (C) 2015-2015 by University of Utah.  All rights
   * reserved.  You may use, examine, or modify this file only in accordance
   * with the GNU Public License, or, alternately, by special written
   * arrangement with the author.  This file comes with no warranties.  If you
   * use it and something breaks, you are solely responsible for cleaning up
   * afterwards.
   *
   */

/*** Configuration Management Information:
   * 
   * $Source: /home/cas/templates/auto_uut/RCS/cas_tmpl.h,v $
   * $Author: cas $
   * $Date: 2014/05/02 19:51:11 $
   * $Revision: 1.4 $
   * 
   */

#if !defined(__LUF_OPTS_H__)
#define __LUF_OPTS_H__

/*** **************************************************************************
   *   includes
   * **************************************************************************
   */


namespace llvm { 
namespace lli_undef_fix { 


/*** **************************************************************************
   *   declarations
   * **************************************************************************
   */

extern bool opt_antidote_select;
extern bool opt_antidote_and_or;
extern bool opt_print_new_poison;
extern bool opt_poison_eq_undef;
extern bool opt_poison_eq_undef_early;
extern bool opt_return_if_div_0;

/*** ==========================================================================
   *   function prototypes
   */

void parse_opts();

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

/*** **************************************************************************
   *   end of file
   * **************************************************************************
   */
} // namespace lli_undef_fix
} // namespace llvm

#endif /* !defined(__LUF_OPTS_H__) */

