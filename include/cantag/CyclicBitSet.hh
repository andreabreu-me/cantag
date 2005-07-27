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

#ifndef CYCLIC_BIT_SET
#define CYCLIC_BIT_SET

#include <cantag/Config.hh>
#include <cantag/Socket.hh>

#include <cassert>
#include <bitset>

namespace Cantag {
  /**
   * A simple extension of the STL bitset that supports some cyclic
   * operations.
   *
   * \todo regression test test all the operations, copy constructor,
   * and the additional methods.
   */
  template<int BIT_COUNT>
  class CyclicBitSet : public std::bitset<BIT_COUNT> {
  private:
    size_t m_rotation;
    bool m_invalid;

  public:
    CyclicBitSet();
    CyclicBitSet(unsigned long val);
    CyclicBitSet(const CyclicBitSet<BIT_COUNT>& o);

    void reset();

    void SetInvalid() { m_invalid = true; }
    
    bool IsInvalid() { return m_invalid; }

    /**
     * Return the number of bits that this bit set has been rotated 
     */
    size_t GetRotation() { return m_rotation; }

    /**
     * Rotate the bit field so that the bit at position i goes to
     * position (i + count) % BIT_COUNT.
     */
    void RotateLeft(size_t count);

    /**
     * Rotate the bit field so that the bit at position
     * (i+count)%BIT_COUNT goes to i.
     */
    void RotateRight(size_t count);

    /**
     * Rotate this bit set so that no other rotation is smaller and
     * return the number of steps required to do so.
     */
    size_t MinRotate();

    /**
     * Rotate this bit step in steps of n bits so that no other rotation
     * is smaller.  Return the number of steps required to do so.
     */ 
    size_t MinRotate(size_t n);

    /**
     * Return true if this bit set rotated left by rot1 is less than
     * this bitset rotated left by rot2.
     */
    bool LessRotate(size_t rot1, size_t rot2) const;

    /**
     * Returns true if the lower bits of this object match the passed
     * object or vice versa if the passed object is longer.
     *
     * \todo Doesn't deal with the passed object being longer
     */ 
    template<int SIZE> bool Equals(const CyclicBitSet<SIZE>& target) const;

    /**
     * Get the index'th symbol (of size count bits) from the payload.
     */
    unsigned int GetSymbol(size_t symbol_number, size_t symbol_size) const;

    /**
     * Store the given symbol as the index'th symbol of count bits
     */
    void PutSymbol(unsigned int symbol, size_t symbol_number,size_t symbol_size);

    bool operator[](size_t n) const;

    typename std::bitset<BIT_COUNT>::reference operator[](size_t n);
    bool operator<(const CyclicBitSet<BIT_COUNT>& o) const;

    bool operator==(const CyclicBitSet<BIT_COUNT>& o) const;

    bool operator!=(const CyclicBitSet<BIT_COUNT>& o) const;

    CyclicBitSet& operator<<=(size_t shift);

    CyclicBitSet& operator>>=(size_t shift);

    CyclicBitSet& operator<<(size_t shift) const;

    CyclicBitSet& operator>>(size_t shift) const;

    unsigned long to_ulong() const;
    
    int Save(Socket& socket) const;
    CyclicBitSet(Socket& socket);
  };

  template<int BIT_COUNT> int CyclicBitSet<BIT_COUNT>::Save(Socket& socket) const {
    int count = socket.Send((int)m_rotation);
    unsigned char* buffer = new unsigned char[BIT_COUNT / 8 + 1];
    for(int i=0;i<BIT_COUNT;++i) {
      if (i%8==0) { buffer[i/8] = 0;}
      if ((*this)[i]) { buffer[i/8] |= (1 << (i%8)); }
    } 
    count += socket.Send(buffer,BIT_COUNT/8+1);
    return count;
  }


  template<int BIT_COUNT> CyclicBitSet<BIT_COUNT>::CyclicBitSet() : std::bitset<BIT_COUNT>(), m_rotation(0), m_invalid(false) {};

  template<int BIT_COUNT> CyclicBitSet<BIT_COUNT>::CyclicBitSet(unsigned long val) : std::bitset<BIT_COUNT>(val), m_rotation(0), m_invalid(false) {};

  template<int BIT_COUNT> CyclicBitSet<BIT_COUNT>::CyclicBitSet(const CyclicBitSet<BIT_COUNT>& o) : std::bitset<BIT_COUNT>(o), m_rotation(o.m_rotation), m_invalid(false) {};

  template<int BIT_COUNT> void CyclicBitSet<BIT_COUNT>::reset() {
    std::bitset<BIT_COUNT>::reset();
  }
  template<int BIT_COUNT> void CyclicBitSet<BIT_COUNT>::RotateLeft(size_t count) {
    m_rotation += count;
    m_rotation %= BIT_COUNT;
  }

  template<int BIT_COUNT> void CyclicBitSet<BIT_COUNT>::RotateRight(size_t count) {
    m_rotation -= count;
    while (m_rotation < 0) m_rotation+=BIT_COUNT;
  }


  template<int BIT_COUNT> size_t CyclicBitSet<BIT_COUNT>::MinRotate() {
    return MinRotate(1);
  }


  template<int BIT_COUNT> size_t CyclicBitSet<BIT_COUNT>::MinRotate(size_t n) {
    size_t minindex = 0;
    for(size_t i=n;i<BIT_COUNT;i+=n) {
      if (LessRotate(i,minindex)) {
	minindex=i;
      }
    }
    RotateLeft(minindex);
    return minindex/n;
  }
  template<int BIT_COUNT> bool CyclicBitSet<BIT_COUNT>::LessRotate(size_t rot1, size_t rot2) const {
    for(size_t i=BIT_COUNT;i>0;i--) {
      int rot1_index = (i-1+rot1) % BIT_COUNT;
      int rot2_index = (i-1+rot2) % BIT_COUNT;
      if ( !(*this)[rot1_index] && (*this)[rot2_index] ) {
	return true;
      }
      else if ( (*this)[rot1_index] && !(*this)[rot2_index] ) {
	return false;
      }
    }
    return false;
  }

  template<int BIT_COUNT> template<int SIZE>  bool CyclicBitSet<BIT_COUNT>::Equals(const CyclicBitSet<SIZE>& target) const {
    for(int i=0;i<SIZE;i++) {
      if (target[i] != (*this)[i]) {
	return false;
      }
    }
    return true;
  }

  template<int BIT_COUNT> unsigned int CyclicBitSet<BIT_COUNT>::GetSymbol(size_t symbol_number, size_t symbol_size) const {
    assert(symbol_size < 8*sizeof(unsigned int));
    unsigned int result = 0;
    for(size_t i=symbol_size;i>0;i--) {
      result <<= 1;
      result |= (*this)[symbol_number*symbol_size+i-1];
    }
    return result;
  }

  template<int BIT_COUNT> void CyclicBitSet<BIT_COUNT>::PutSymbol(unsigned int symbol, size_t symbol_number,size_t symbol_size) {
    assert(symbol_size < 8*sizeof(unsigned int));
    for(unsigned int i=0;i<symbol_size;i++) {
      bool value = (symbol & 0x1 == 0x1);
      (*this)[symbol_number*symbol_size+i] = value;
      symbol>>=1;
    }
  }


  template<int BIT_COUNT> bool CyclicBitSet<BIT_COUNT>::operator[](size_t n) const {
    return std::bitset<BIT_COUNT>::operator[]((n+m_rotation) % BIT_COUNT);
  }


  template<int BIT_COUNT> typename std::bitset<BIT_COUNT>::reference CyclicBitSet<BIT_COUNT>::operator[](size_t n) {
    return std::bitset<BIT_COUNT>::operator[]((n+m_rotation) % BIT_COUNT);
  }

  template<int BIT_COUNT> bool CyclicBitSet<BIT_COUNT>::operator<(const CyclicBitSet<BIT_COUNT>& o) const {
    for(unsigned int i=BIT_COUNT;i>0;i--) {
      if ( !(*this)[i-1] && o[i-1] ) {
	return true;
      }
      else if ( (*this)[i-1] && !o[i-1] ) {
	return false;
      }
    }
  }
  template<int BIT_COUNT> bool CyclicBitSet<BIT_COUNT>::operator==(const CyclicBitSet<BIT_COUNT>& o) const {
    for(unsigned int i=0;i<BIT_COUNT;i++) {
      if ( (*this)[i] != o[i]) { 
	return false;
      }
    }
    return true;
  }
  template<int BIT_COUNT> bool CyclicBitSet<BIT_COUNT>::operator!=(const CyclicBitSet<BIT_COUNT>& o) const {
    return !(*this == o);
  }

  template<int BIT_COUNT> CyclicBitSet<BIT_COUNT>& CyclicBitSet<BIT_COUNT>::operator<<=(size_t shift) {
    for(unsigned int i=BIT_COUNT-shift;i<BIT_COUNT;i++) { 
      (*this)[i] = false;
    }
    RotateRight(shift);    
    return *this;
  }

  template<int BIT_COUNT> CyclicBitSet<BIT_COUNT>& CyclicBitSet<BIT_COUNT>::operator>>=(size_t shift) {
    for(unsigned int i=0;i<shift;i++) {
      (*this)[i] = false;     
    }
    RotateLeft(shift);
    return *this;
  }
  template<int BIT_COUNT> CyclicBitSet<BIT_COUNT>& CyclicBitSet<BIT_COUNT>::operator<<(size_t shift) const {
    return CyclicBitSet<BIT_COUNT>(*this)<<=shift;
  }
  template<int BIT_COUNT> CyclicBitSet<BIT_COUNT>& CyclicBitSet<BIT_COUNT>::operator>>(size_t shift) const {
    return CyclicBitSet<BIT_COUNT>(*this)>>=shift;
  }

  template<int PAYLOAD_SIZE> std::ostream& operator<<(std::ostream& os, const CyclicBitSet<PAYLOAD_SIZE>& x) {
    for(size_t i=0;i<PAYLOAD_SIZE;i++) {
      os << x[PAYLOAD_SIZE-1-i];
    }
    return os;
  };

  template<int BIT_COUNT> unsigned long  CyclicBitSet<BIT_COUNT>::to_ulong() const {
    return std::bitset<BIT_COUNT>::to_ulong();
  }


}
#endif//CYCLIC_BIT_SET