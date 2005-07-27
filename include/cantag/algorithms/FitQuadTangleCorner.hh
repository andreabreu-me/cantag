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

#ifndef FITQUADTANGLECORNER_GUARD
#define FITQUADTANGLECORNER_GUARD

#include <cantag/QuadTangle.hh>
#include <cantag/entities/Entity.hh>
#include <cantag/entities/ContourEntity.hh>
#include <cantag/Function.hh>

namespace Cantag {
  class FitQuadTangleCorner : public Function1<ContourEntity,ShapeEntity<QuadTangle> >{
  public:
    bool operator()(const ContourEntity& contour, ShapeEntity<QuadTangle>& shape) const;
  };
}

#endif//FITQUADTANGLECORNER_GUARD