/**
 * $Header$
 */
#include <FileImageSource.hh>

FileImageSource::FileImageSource(char* filename) : m_original(new Image(filename)), m_buffer(new Image(filename)) {}    

FileImageSource::~FileImageSource() { }

void FileImageSource::Next() {
  if (m_buffer != NULL) {
    delete m_buffer;
  }
  m_buffer = new Image(*m_original);
}

Image* FileImageSource::GetBuffer() {
  return m_buffer;
}
