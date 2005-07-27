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
#ifndef CAMERA_GUARD
#define CAMERA_GUARD

#include <cantag/Config.hh>
#include <cantag/Image.hh>
#include <cantag/Ellipse.hh>
#include <cantag/QuadTangle.hh>
#include <vector>

namespace Cantag {

  /**
   * A camera model.  Use this class to convert from NPCF to Image co-ordinates and back again
   *
   * \todo add loading the camera parameters from disk
   */
  class Camera {
  private:
    float m_extrinsic[16];
    float m_intrinsic[9];
  
    // radial distortion co-efficients
    float m_r2;
    float m_r4;
    float m_r6;

    // tangential distortion co-efficients
    float m_d1;
    float m_d2;

  public:
    /**
     * Create a camera with default options - unit size, zero skew,
     * principle point at 0.5,0.5 and no distortion.
     */
    Camera();
    ~Camera();

    /** 
     * Set the extrinsic parameters for this camera - this is a 3d
     * (homogenous) transformation matrix from camera co-ordinates to world
     * co-ordinates
     */ 
    void SetExtrinsic(float extrinsic[16]);

    /**
     * Set the intrinsic parameters for this camera. This is a matrix
     * containing x and y scale factors, principle point and skew
     */
    void SetIntrinsic(float intrinsic[9]);

    /**
     * Set the intrinsic parameters for this camera to the given values.
     */
    void SetIntrinsic(float xscale, float yscale, float principlex, float principley, float skew);

    /**
     * Set the radial distortion parameters.  This method accepts
     * co-efficients for the radius squared, to the four and to the six.
     */
    void SetRadial(float r2,float r4,float r6);

    /**
     * Set the tangential distortion co-efficients for this camera.
     */
    void SetTangential(float d1, float d2);


    /**
     * Convert a set of image co-ordinates to normalised principle
     * co-ordinate frame (NPCF) points.  This involves removing the
     * distortion predicted by the parameters set and removing the
     * extrinsic parameters of the camera.
     *
     * Uses an approximation to the inverse distrortion calculation.
     * It is based on the first and second terms of a taylor expansion
     * of the distortion equeation. Neglection of the higher order terms
     * reduces accuracy, but not noticeably AFAICT. This model assumes 
     * only 4th order radial distortion (i.e. no r^n for n>4) and will
     * ignore the 6th order term if set. Similarly the approximation
     * cannot cope with tangential distortion. It is however, fast to compute
     * and should suffice since 99.99% of digital cameras really only have
     * 2nd order radial distortion.
     *
     * More details in "Precise Radial Un-distortion of Images",
     * John Mallon and  Paul F. Whelan, 7th International Conf. on
     * Pattern Recognition (ICPR'04), Cambridge, UK. August 2004
     *
     * Contains code by Rob Harle <rkh23@cam.ac.uk>
     */
    void ImageToNPCF(std::vector<float>& points) const;

    /**
     * Convert a set of image co-ordinates to normalised principle
     * co-ordinate frame (NPCF) points.  
     *
     * \sa #ImageToNPCF()
     */
    void ImageToNPCF(float* points, int num_points) const;

    /**
     * Convert a set of normalised principle co-ordinate frame (NPCF)
     * points to image points.  This involves applying the correct
     * distortion predicted by the camera parameters and applying the
     * extrinsic parameters of the camera.
     */
    void NPCFToImage(std::vector<float>& points) const;  

    /**
     * Convert a set of normalised principle co-ordinate frame (NPCF)
     * points to image points. 
     *
     * \sa #NPCFToImage()
     */
    void NPCFToImage(float* points, int num_points) const;  

    /**
     * Remove radial and tangential distortion from the whole image.
     */
    void UnDistortImage(Image<Colour::Grey>& image) const;

    /**
     * Convert the camera coordinates (given as an array of triples) to world coordinates
     */
    void CameraToWorld(float* points, int num_points) const;

    /**
     * Convert these camera coordinates to world coordinates
     */
    void CameraToWorld(float x, float y, float z, float* rx, float* ry, float *rz) const;

    /**
     * Draw this ellipse on the image given after removing the NPCF scale factors from it
     */
    void DrawEllipse(Image<Colour::Grey>& image, const Ellipse& ellipse) const;

    void Draw(Image<Colour::Grey>& image, const Ellipse& ellipse) const;
    void Draw(Image<Colour::Grey>& image, const QuadTangle& ellipse) const;

    /**
     * Draw this quadtangle on the image given after removing the NPCF scale factors from it.
     */
    void DrawQuadTangle(Image<Colour::Grey>& image, const QuadTangle& qt) const;

    
  };
}

#else
namespace Cantag {
  class Camera;
}
#endif//CAMERA_GAURD
