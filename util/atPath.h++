
#ifndef AT_PATH_HPP
#define AT_PATH_HPP


#include "atFile.h++"
#include "atList.h++"
#include "atNotifier.h++"
#include "atOSDefs.h++"
#include "atString.h++"


#define DELIMITER_STRING "/\\"


class ATLAS_SYM atPath : public atNotifier
{
   public:

      static bool       createDirectory(atString path);
      static atList *   listFiles(atString path);

      static atString   getFilenameFromPath(atString path);
      static atString   getExtension(atString path, atString delimiter);
      static atString   getFilenameWithoutExtension(atString path,
                                                    atString delimiter);
};

#endif
