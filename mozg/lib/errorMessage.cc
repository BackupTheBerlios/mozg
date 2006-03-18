// $Id: errorMessage.cc,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// 
// send error message



#include "mozg/errorMessage.hh"


namespace mozg {

  void sendErrorMessage(char* message,
			int err_num)
    {
      std::cerr << "\n" << message << "\nPress any key...";
      getchar();
      if (err_num==1) exit(1);
    }

}
