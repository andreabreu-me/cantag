#ifndef TEMPLATE_GUARD
#define TEMPLATE_GUARD

#include <tripover/Config.hh>
#include <tripover/Image.hh>
#include <tripover/Camera.hh>

/**
 * A template code.  Handles correlation with other templates and the
 * like.
 *
 * \todo{regression test: create various images which we expect various correlation coefficients from and try them}
 */
class Template {
private:
  float m_average;
  unsigned int m_size;
  unsigned int m_subsample;
  uchar* m_values;
  float m_sigma;
  Image* m_original;

public:
  char* m_filename;
  Template(char* filename, int size, int subsample);
  ~Template();
  float Correlate(const Image& image, const float transform[16], const Camera& camera) const;
  float Correlate(const Template& t) const;
  bool operator<(const Template& t);
  void Draw2D(Image& image);
private:
  void calculate_mean_sigma(const unsigned char* values, float* stddev, float* mean) const;
  float Correlate(const unsigned char* values) const;
};


#endif//TEMPLATE_GUARD