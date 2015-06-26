/*** 
   * Program Name: lli_undef_fix 
   *
   * \file LUF_opts.cpp 
   *
   * \brief Code to handle options for lli_undef_fix (LUF) extensions to LLVM.
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

#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "llvm/Support/LUF_opts.h"

/*++ **************************************************************************
   *   declarations
   * **************************************************************************
   */

/** do not propogate poison from the unused branch of a select opcode */
bool llvm::lli_undef_fix::opt_antidote_select= false;

/** use short circuit evaluation to remove poison from unused operands of and
and or opcodes.  */ 
bool llvm::lli_undef_fix::opt_antidote_and_or= false;

/** prints a message when a new poison value is generated */
bool llvm::lli_undef_fix::opt_print_new_poison= false;

/** implements David Majnemer's and Nuno Lopes' April 2015 proposal that
   poison should be merged with undef.
*/
bool llvm::lli_undef_fix::opt_poison_eq_undef= false;

/** adds an interpretation of David's & Nuno's plan with early
    undef-to-random-value resolution.
 */
bool llvm::lli_undef_fix::opt_poison_eq_undef_early= false;

/** if and IR opcode attempts division by zero, return instead of crashing.
 */
bool llvm:lli_undef_fix::opt_return_if_div_0;

typedef struct {
   const char* name;
   bool* dest_ptr;
   bool default_val;
} filelocal_opt_t;

const filelocal_opt_t opt_array[]= {
  { "antidote_select", 
    &llvm::lli_undef_fix::opt_antidote_select, 
    false },
  { "antidote_and_or", 
    &llvm::lli_undef_fix::opt_antidote_and_or, 
    false },

  { "print_new_poison", 
    &llvm::lli_undef_fix::opt_print_new_poison, 
    false },

  { "poison_eq_undef", 
    &llvm::lli_undef_fix::opt_poison_eq_undef, 
    false },

  { "poison_eq_undef_early", 
    &llvm::lli_undef_fix::opt_poison_eq_undef_early, 
    false },

  { "return_if_div_0", 
    &llvm::lli_undef_fix::opt_return_if_div_0, 
    false },

  // end of the list
  { NULL, NULL, false }
};

/*++ ==========================================================================
   *   un-submoduled function prototypes
   * ==========================================================================
   */

/*++ **************************************************************************
   *   source code
   * **************************************************************************
   */

// ----------------------------------------------------------------------------
///  \fn parse_opts()
// ----------------------------------------------------------------------------
/*** \brief parse LLI Undef Fix-specific options
   *
   * \b Detailed_Description: 
   *
   * These options are usually passed in via the LLI_LUF_OPTS environment
   * variable.
   *
   * \b Method: 
   *
   * \b Reentrancy: 
   *
   * \return void
   *
   */
void llvm::lli_undef_fix::parse_opts()
{{
  const char* ENV_VAR_NAME= "LLI_LUF_OPTS";
  char* env_val= getenv( ENV_VAR_NAME );
  const char* INDENT= "  ";

  /* initialize all options to their default values */
  {
    const filelocal_opt_t* opt_array_p= NULL;
    for ( opt_array_p= &opt_array[0]; 
	opt_array_p->name != NULL; 
	opt_array_p++ ) {
      *(opt_array_p->dest_ptr)= opt_array_p->default_val;
    }
  }
  
  /* read the options from the environment */
  bool need_to_exit= false;
  char* saveptr= NULL;
  const char* opt= strtok_r( env_val, ",", &saveptr );
  while ( opt != NULL ) {
    const filelocal_opt_t* opt_array_p= NULL;
    bool found= false;
    for ( opt_array_p= &opt_array[0]; 
	opt_array_p->name != NULL; 
	opt_array_p++ ) {
       if ( 0 == strcmp( opt, opt_array_p->name ) ) {
	 found= true;
	 *(opt_array_p->dest_ptr)= !(opt_array_p->default_val);
       }
    }
    if ( ! found ) {
      std::cerr << "do not understand option \"" << opt << 
	  "\" in environment variable \"" << ENV_VAR_NAME << "\". \n";
      need_to_exit= true;
    }

    /* increment */
    opt= strtok_r( NULL, ",", &saveptr );
  }

  if ( need_to_exit ) {
    std::cerr << "Too many errors, exiting. \n";
    exit( EXIT_FAILURE );
  }

  /* print out the options in force */
  { 
    const filelocal_opt_t* opt_array_p= NULL;

    std::cout << "LUF Option settings via " << ENV_VAR_NAME << ": \n";

    for ( opt_array_p= &opt_array[0]; 
	opt_array_p->name != NULL; 
	opt_array_p++ ) {
      std::cout << INDENT << opt_array_p->name << "=" << 
	  *(opt_array_p->dest_ptr) << " (default=" << 
	  opt_array_p->default_val << ") \n";
    }
    std::cout << INDENT << "(end of options) \n\n";
  }

  /* clean up and return */
  return;
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

