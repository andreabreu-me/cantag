/**
 * $Header$
 */ 

#ifndef TEXT_OUTPUT_MECHANISM_GUARD
#define TEXT_OUTPUT_MECHANISM_GUARD

#include <ostream>
#include <tripover/LocatedObject.hh>

class TextOutputMechanism {
private:
  std::ostream& m_ostream;

public:
  
  TextOutputMechanism(std::ostream& ostream) : m_ostream(ostream) {};
  
  inline void FromImageSource(const Image& image) {};
  inline void FromThreshold(const Image& image) {};
  inline void FromContourTree(const ContourTree& contours) {};
  inline void FromRemoveIntrinsic(const ContourTree& contours) {};
  template<class ShapeType> inline void FromShapeTree(const ShapeTree<ShapeType>& shapes) {};
  template<int PAYLOADSIZE> void FromTag(const WorldState<PAYLOADSIZE>& world);
};

template<int PAYLOADSIZE> void TextOutputMechanism::FromTag(const WorldState<PAYLOADSIZE>& world) {
  for(typename std::vector<LocatedObject<PAYLOADSIZE>*>::const_iterator i = world.GetNodes().begin();
      i != world.GetNodes().end();
      ++i) {
    LocatedObject<PAYLOADSIZE>* loc = *i;
    m_ostream << "Found tag: " << *(loc->tag_code) << 
      " Location: " << loc->location[0] << " " << loc->location[1] << " " << loc->location[2] <<
      " Normal: "   << loc->normal[0] << " " << loc->normal[1] << " " << loc->normal[2] << std::endl;
  }
}

#endif//TEXT_OUTPUT_MECHANISM_GUARD