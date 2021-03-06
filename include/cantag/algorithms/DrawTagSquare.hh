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

#ifndef DRAW_TAG_SQUARE_GUARD
#define DRAW_TAG_SQUARE_GUARD

#include <cantag/Config.hh>
#include <cantag/entities/DecodeEntity.hh>
#include <cantag/Function.hh>
#include <cantag/Image.hh>
#include <cantag/TagSquare.hh>
#include <cantag/SpeedMath.hh>

namespace Cantag {
  
  /**
   * Algorithm for drawing a square tag to an image. This is a
   * helper object which you can create using the inline template
   * function DrawTagSquare which will infer template parameters for
   * you.
   */
  template<int SIZE>
  class DrawTagSquareObj : public Function<TL1(DecodeEntity<SIZE*SIZE - (SIZE*SIZE)%2>), TypeList<Image<Pix::Sze::Byte1,Pix::Fmt::Grey8> > > {
  private:
    const TagSquare<SIZE>& m_tagspec;
    static const int PayloadSize = TagSquare<SIZE>::PayloadSize;

    typedef typename DecodeEntity<PayloadSize>::Data DecodeData; // needed to convince the compiler to parse this file

  public:
    DrawTagSquareObj(const TagSquare<SIZE>& tagspec) : m_tagspec(tagspec) {}
    
    bool operator()(const DecodeEntity<PayloadSize>& code, Image<Pix::Sze::Byte1,Pix::Fmt::Grey8>& image) const;
  };

  template<int SIZE> bool DrawTagSquareObj<SIZE>::operator()(const DecodeEntity<PayloadSize>& code, Image<Pix::Sze::Byte1,Pix::Fmt::Grey8>& image) const {
    if (code.GetPayloads().size() == 0) return false;

    typename std::vector<DecodeData*>::const_iterator i = code.GetPayloads().begin();
    DecodeData* chosen = *i;

    for(;i!=code.GetPayloads().end();++i) {
      DecodeData* candidate = *i;
      if (chosen->confidence < candidate->confidence) chosen = candidate;
    }


    int x0 = 0;
    int y0 = 0;
    int x1 = image.GetWidth()-1;
    int y1 = 0;
    int x2 = image.GetWidth()-1;
    int y2 = image.GetHeight()-1;
    int x3 = 0;
    int y3 = image.GetHeight()-1;

    float size = (image.GetWidth() < image.GetHeight() ? image.GetWidth()-1 : image.GetHeight()-1)/2.f;

    float cell_size = 2.f*size / ((float)SIZE+2.f);

    image.DrawFilledQuadTangle(x0, y0,
			       x1, y1,
			       x2, y2,
			       x3, y3,COLOUR_BLACK);

    int ptr = 0;
    for(int i=0;i<SIZE*SIZE - (SIZE*SIZE % 2);++i) {
      float ubase = m_tagspec.GetX0(i)*size + size;
      float vbase = m_tagspec.GetY0(i)*size + size;

      int u0 = Round(ubase);
      int v0 = Round(vbase);

      int u1 = Round(ubase + cell_size);
      int v1 = Round(vbase);

      int u2 = Round(ubase + cell_size);
      int v2 = Round(vbase + cell_size);

      int u3 = Round(ubase);
      int v3 = Round(vbase + cell_size);

      Pixel<Pix::Fmt::Grey8> colour = chosen->payload[ptr] ? COLOUR_BLACK : COLOUR_WHITE;

      // this one just draws a greyscale range starting at black and going to white
      //Pixel<Pix::Fmt::Grey8> colour((int)((float)ptr/(float)(SIZE*SIZE - (SIZE*SIZE % 2)) * 128)+128);

      image.DrawFilledQuadTangle(u0,v0,
				 u1,v1,
				 u2,v2,
				 u3,v3,
				 colour);
      ++ptr;
    }
    return true;
  }

  template<int SIZE>
  inline
  DrawTagSquareObj<SIZE> DrawTagSquare(const TagSquare<SIZE>& tagspec) {
    return DrawTagSquareObj<SIZE>(tagspec);
  }

  template<int SIZE>
  inline
  DrawTagSquareObj<SIZE> DrawTag(const TagSquare<SIZE>& tagspec) {
    return DrawTagSquareObj<SIZE>(tagspec);
  }
}

#endif//DRAW_TAG_SQUARE_GUARD
