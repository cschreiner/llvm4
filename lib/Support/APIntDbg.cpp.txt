//===-- APIntDbg.cpp - temporary parts of class APInt used for debugging --===// 
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements a class to represent arbitrary precision integer
// constant values and provide a variety of arithmetic operations on them.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/APInt.h"
#include "llvm/ADT/APIntPoison.h"

#if 0
  #include "llvm/ADT/FoldingSet.h"
  #include "llvm/ADT/Hashing.h"
  #include "llvm/ADT/SmallString.h"
  #include "llvm/ADT/StringRef.h"
  #include "llvm/Support/Debug.h"
  #include "llvm/Support/ErrorHandling.h"
  #include "llvm/Support/MathExtras.h"
  #include "llvm/Support/raw_ostream.h"
  #include <cmath>
  #include <cstdlib>
  #include <cstring>
  #include <limits>
#endif

#include <iostream>
#include <stdlib.h>

// ############################################################################
namespace llvm {
  /// \brief Fast internal constructor
  ///
  /// This constructor is used only internally for speed of construction of
  /// temporaries. It is unsafe for general use so it is not public.
  APInt::APInt(uint64_t *val, unsigned bits) : 
      BitWidth(bits), pVal(val), poisoned(false) {
    //std::cout << "starting APInt::APInt( uint64_t* val, unsigned bits )\n";;
  }

  /// \brief Create a new APInt of numBits width, initialized as val.
  ///
  /// If isSigned is true then val is treated as if it were a signed value
  /// (i.e. as an int64_t) and the appropriate sign extension to the bit width
  /// will be done. Otherwise, no sign extension occurs (high order bits beyond
  /// the range of val are zero filled).
  ///
  /// \param numBits the bit width of the constructed APInt
  /// \param val the initial value of the APInt
  /// \param isSigned how to treat signedness of val

  APInt::APInt(unsigned numBits, uint64_t val, bool isSigned ) 
      : BitWidth(numBits), VAL(0), poisoned(false) {
    assert(BitWidth && "bitwidth too small");
    //std::cout << "starting APInt::APInt( unsigned, uint64_t, bool)...\n";; 
    bool inDebugMode= false;
    if ( NULL != getenv("APIntDbg") )  {
      inDebugMode= true;
      //std::cout << "starting APInt::APInt( unsigned, uint64_t, bool)...\n";;
    }
    if (isSingleWord())
      VAL = val;
    else
      initSlowCase(numBits, val, isSigned);
    clearUnusedBits();
    //std::cout << "stopping APInt::APInt(unsigned, uint64_t, bool), val=" << 
    //    toString(10,false) << ".\n";;
  }


  /// Simply makes *this a copy of that.

  APInt::APInt(const APInt &that) 
      : BitWidth(that.BitWidth), VAL(0), poisoned(that.poisoned) {
    assert(BitWidth && "bitwidth too small");
    poisoned= that.poisoned;
    if (isSingleWord())
      VAL = that.VAL;
    else
      initSlowCase(that);
  }

  /// \brief Move Constructor.

  APInt::APInt(APInt &&that) 
      : BitWidth(that.BitWidth), VAL(that.VAL), poisoned(that.poisoned) {
    that.BitWidth = 0;
  }



// ############################################################################
} // namespace llvm


