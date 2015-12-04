/*** 
   * Program Name: <$project_name>
   *
   * File Name: LUF_etc.h
   *
   * \brief: miscelaneous facilities related to lli_undef_fix (LUF)
   *
   * \b Detailed_Description:
   *
   *
   * Copyright (C) 2015-2015 by Christian A. Schreiner.  All rights
   * reserved.  You may use, examine, or modify this file only in accordance
   * with the GNU Public License, or, alternately, by special written
   * arrangement with the author.  This file comes with no warranties.  If you
   * use it and something breaks, you are solely responsible for cleaning up
   * afterwards.
   *
   */

/*** Configuration Management Information:
   * 
   * $Source: /net/home/cas/templates/auto/RCS/cas_tmpl.h,v $
   * $Author: cas $
   * $Date: 2013/02/04 15:57:01 $
   * $Revision: 1.4 $
   * 
   */

#if !defined(__LUF_ETC_H__)
#define __LUF_ETC_H__

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

/*** ==========================================================================
   *   function prototypes
   */

void exit_due_to_poison();

/*** **************************************************************************
   *   end of file
   * **************************************************************************
   */
} // namespace lli_undef_fix
} // namespace llvm

#endif /* !defined(__LUF_ETC_H__) */

