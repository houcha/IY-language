#ifndef STRVIEW_OUT_HPP
#define STRVIEW_OUT_HPP


#define STRVIEW "%.*s"
#define VIEW(str) static_cast<int>(str.size()), str.data()
#define VIEW_LEN(str, len) static_cast<int>(len), str.data()


#endif // STRVIEW_OUT_HPP

