/*
  Copyright (C) 2004 Andrew C. Rice

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Email: acr31@cam.ac.uk
*/

/**
 * $Header$
 */

#include <cantag/Coder.hh>
#include <cantag/TripOriginalCoder.hh>
#include <cantag/ParityCoder.hh>
#include <cantag/SymbolChunkCoder.hh>
#include <cantag/SCCCoder.hh>
#include <cantag/CyclicBitSet.hh>

#include <boost/test/unit_test.hpp>
#include <boost/random.hpp>
#include <iostream>

using boost::unit_test_framework::test_suite;
using namespace Cantag;

template<int PAYLOAD_SIZE> void tryrotations(const CyclicBitSet<PAYLOAD_SIZE>& test_value,
					     const CyclicBitSet<PAYLOAD_SIZE>& target_code,
					     bool claimFailOK,
					     const Coder<PAYLOAD_SIZE>& coder) {
  // try it over all rotations - should come back identically
  for(unsigned int i=0;i<PAYLOAD_SIZE;i+=coder.GetSymbolSize()) {
    CyclicBitSet<PAYLOAD_SIZE> temp(test_value);
    temp.RotateLeft(i);
    int rotation = coder.DecodePayload(temp);
    if (rotation >= 0) {
      if (temp != target_code) {
	throw "Coder failed to decode the rotated code and it didn't notice.";
      }
    }
    else {
      if (!claimFailOK) {
	throw "Coder claimed that it failed to decode the rotated code";
      }
    }
  }
}
					     
template<int PAYLOAD_SIZE> void codertest(const Coder<PAYLOAD_SIZE>& coder) {
  
  boost::mt19937 rng;
  boost::uniform_int<> bitsource(0,1);
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > bits(rng, bitsource);

  boost::uniform_int<> indexsource(0,PAYLOAD_SIZE-1);
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > indices(rng, indexsource);
  
  // generate a code word
  CyclicBitSet<PAYLOAD_SIZE> code;
  for(unsigned int i=0;i<PAYLOAD_SIZE;i++) {
    code[i] = bits();
  }

  coder.EncodePayload(code);
  const CyclicBitSet<PAYLOAD_SIZE> encoded_value(code);

  coder.DecodePayload(code);
  const CyclicBitSet<PAYLOAD_SIZE> target_code(code);

  tryrotations<PAYLOAD_SIZE>(encoded_value,target_code,false,coder);

  for(int i=0;i<1000;i++) {
    if (coder.IsErrorCorrecting()) {
      // if the code is error correcting the try inserting hamming dist/2
      // errors and seeing if, under all rotations the code creates the
      // correct value
      CyclicBitSet<PAYLOAD_SIZE> temp(encoded_value);
      for(int i=0;i<coder.GetHammingDistanceBits()/2;i++) {
	int index = indices();
	temp[index] = !temp[index];
      }
      tryrotations<PAYLOAD_SIZE>(temp,target_code,false,coder);
    }
    else {
      // if the code is error detecting then try inserting the 1-hamming
      // distance errors and seeing, if under all rotations, the code spots the error
      CyclicBitSet<PAYLOAD_SIZE> temp(encoded_value);
      for(int i=0;i<coder.GetHammingDistanceBits()-1;i++) {
	int index = indices();
	temp[index] = !temp[index];
      }
      tryrotations<PAYLOAD_SIZE>(temp,target_code,true,coder);
    }
  }
}


void test_trip() {
  TripOriginalCoder<30,2,2> t;
  codertest<30>(t);
}

void test_parity() {
  ParityCoder<30> t;
  codertest<30>(t);
}

void test_symbolchunk() {
  SymbolChunkCoder<30,5> t;
  codertest<30>(t);
}

void test_scc() {
  SCCCoder<2,17,3,6> t;
  codertest<34>(t);
}

test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
  test_suite* test= BOOST_TEST_SUITE( "Coder test" );

  test->add( BOOST_TEST_CASE( &test_trip ) );
  test->add( BOOST_TEST_CASE( &test_parity ) );
  test->add( BOOST_TEST_CASE( &test_symbolchunk ) );
  test->add( BOOST_TEST_CASE( &test_scc ) );

  return test;
}

