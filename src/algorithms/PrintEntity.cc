/*
  Copyright (C) 2005 Andrew C. Rice

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

#include <cantag/algorithms/PrintEntity.hh>

namespace Cantag {

  bool PrintEntityContour::operator()(ContourEntity& contour) const {
    for(std::vector<float>::const_iterator i = contour.GetPoints().begin();
	i != contour.GetPoints().end();
	++i) {
      const float x = *i;
      ++i;
      const float y = *i;
      m_output << x << " " << y << std::endl;
    }    
    m_output << std::endl;
    return true;
  }

  bool PrintEntityConvexHull::operator()(const ContourEntity& contour, ConvexHullEntity& convexhull) const {
    const std::vector<float> points = contour.GetPoints();
    for(std::vector<int>::const_iterator i = convexhull.GetIndices().begin();
	i != convexhull.GetIndices().end();
	++i) {
      const int index = *i;
      m_output << points[2*index] << " " << points[2*index+1] << std::endl;
    }    
    m_output << std::endl;
    return true;
  }

  bool PrintEntityShapeSquare::operator()(ShapeEntity<QuadTangle>& shape) const {
    const QuadTangle* quad = shape.GetShape();
    m_output << quad->GetX0() << " " << quad->GetY0() << " " << quad->GetIndex0() << std::endl;
    m_output << quad->GetX1() << " " << quad->GetY1() << " " << quad->GetIndex1() << std::endl;
    m_output << quad->GetX2() << " " << quad->GetY2() << " " << quad->GetIndex2() << std::endl;
    m_output << quad->GetX3() << " " << quad->GetY3() << " " << quad->GetIndex3() << std::endl;
    m_output << std::endl;
    return true;
  }

  bool PrintEntityShapeCircle::operator()(ShapeEntity<Ellipse>& shape) const {
    const Ellipse* ell = shape.GetShape();
    m_output << ell->GetX0() << " " << ell->GetY0() << " " << ell->GetWidth() << " " << ell->GetHeight() << " " << ell->GetAngle() << std::endl;
    m_output << std::endl;
    return true;
  }

  bool PrintEntityTransform::operator()(TransformEntity& te) const {
    const Transform* t = te.GetPreferredTransform();
    if (t==NULL) return false;
    t->Print(m_output);
    return true;
  }

}
