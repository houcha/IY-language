#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>


/// Return file size.
int GetFileSize(FILE* file);


/// Class which eases file content work.
class Text {

  protected:
    /// Raw file content.
    std::string buffer_;

  public:

    Text() : buffer_() {}
    /// Load file content to buffer.
    void Load(FILE* src);

    const char* Data() const { return buffer_.data(); }
};


#endif // TEXT_HPP

