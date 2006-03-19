// $Id: errorMessage.hh,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// errorMessage.hh
// Handle with debug error (sizes mismatches)
//
/****************************************************************************

    Neural Network Library mozg
    Copyright (C) 1999 Alexy Filin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
    02111-1307, USA.
              
 ****************************************************************************/
// send error message

#ifndef MOZG_ERROR_MESSAGE_HH_
#  define MOZG_ERROR_MESSAGE_HH_

#include <iostream>
#include <cstdio>


namespace mozg {

extern void sendErrorMessage(char* message, int err_num);

}

#endif // MOZG_ERROR_MESSAGE_HH_
