/*
  Copyright (C) 2007 Tom Craig

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

  Email: jatc2@cam.ac.uk
*/

/**
 * $Header$
 */

#include <cantag/algorithms/HoughTransform.hh>
#include <cantag/SpeedMath.hh>

#include <iostream>
using std::cout;
using std::endl;
#include <utility>
using std::make_pair;
using std::pair;
#include <vector>
using std::vector;

namespace Cantag
{

  bool HoughTransform::operator()(const MonochromeImage& source, TreeNode<HoughEntity>& result) const
  {
    // Make a note of all the points in our picture.
    vector<pair<int, int> > points;
    for (int y = 0; y < source.GetHeight(); ++y)
      for (int x = 0; x < source.GetWidth(); ++x)
	if (source.GetPixel(x, y))
	  points.push_back(make_pair(x, y));
    cout << "There are " << points.size() << " points in the picture." << endl;
    
    /* 
     * Construct a discrete Hough parameter space.
     */

    // Firstly initialise them constants.
    const unsigned int NUM_THETA_STEPS = m_num_angle_divisions; // TODO: unhardcode
    const unsigned int NUM_R_STEPS = m_num_perpdist_divisions; // TODO: unhardcode
    const float THETA_MAX = 2 * DBL_PI;
    const float THETA_MIN = 0;
    const float R_MAX = source.GetDiagonalLength();
    const float R_MIN = 0 - R_MAX;
    const float THETA_STEP = (THETA_MAX - THETA_MIN) / NUM_THETA_STEPS;
    const float R_STEP = (R_MAX - R_MIN) / NUM_R_STEPS;

    // Now do the accumulator.
    float theta = THETA_MIN;
    float r = R_MIN;
    TreeNode<HoughEntity>* tree_node = &result;
    for (unsigned int i = 0; i < NUM_THETA_STEPS; ++i)
    {
      r = 0;
      for (unsigned int j = 0; j < NUM_R_STEPS; ++j)
      {
	int acc = 0;
	for (unsigned int k = 0; k < points.size(); ++k)
	{
	  int x = points[k].first;
	  int y = points[k].second;
	  float theta0 = theta;
	  float theta1 = theta + THETA_STEP;
	  float r0 = r;
	  float r1 = r + R_STEP;
	  float r_at_theta0 = float(x) * cos(theta0) + float(y) * sin(theta0);
	  float r_at_theta1 = float(x) * cos(theta1) + float(y) * sin(theta1);

	  if ((r_at_theta0 >= r0 && r_at_theta0 <= r1)
	      || (r_at_theta1 >= r0 && r_at_theta1 <= r1)
	      || (r_at_theta0 < r0 && r_at_theta1 > r1)
	      || (r_at_theta0 > r1 && r_at_theta1 < r0))
	  {
	    acc++;
	  }
	}
	
	if (acc > 0)
	{
	  cout << "Adding accumulator of " << acc << " for (" << r << ", " << theta << ")" << endl;
	  tree_node->SetValid(true); // Forgetting this can cause hours of debugging. :-p
	  HoughEntity* h = tree_node->GetNode();
	  h->SetPerpendicularDistance(r); // N.B. we choose to record (r, theta) (and not, say, (r + R_STEP/2, theta + THETA_STEP/2))
	  h->SetAngle(theta);
	  h->SetAccumulator(acc);
	  tree_node = tree_node->AddChild();

	  // update the latest max acc
	  if (m_latest_max_accumulator < acc)
	    m_latest_max_accumulator = acc;
	}

	r += R_STEP;
      }

      theta += THETA_STEP;
    }

    return true;
  }

}
