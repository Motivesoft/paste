/*
 * PASTE
 *
 * A simple executable to act as a partner to the Windows command, CLIP.
 * CLIP takes some input to its input stream and puts it on the Windows clipboard.
 * PASTE does the reverse, it writes the Windows clipboard current textual content and writes it to the output stream.
 * PASTE is designed to look and feel like CLIP, because that seemed a reasonable thing to do.
 *
 * MIT License
 *
 * Copyright (c) 2020 Motivesoft
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <windows.h>

using namespace std;

// Method signatures
int paste();
int readClipboard();
int processData( const HANDLE& h );
void help( const char* app );
bool isHelpRequest( const char* arg );

/// <summary>
/// Entry point.
/// Write the current textual contents of the Windows clipboard to the standard output stream.
/// Expected command command line arguments, either nothing or "/?" to display help. 
/// Any other command line arguments will lead to an error message.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main( int argc, char* argv[] )
{
   int rc = 0;

   if ( argc <= 1 )
   {
      rc = paste();
   }
   else
   {
      // We're going to be showing an message to the user. Reference the executable name when we do this for usage examples
      char appName[ _MAX_FNAME ];
      _splitpath_s( argv[ 0 ], NULL, 0, NULL, 0, appName, _MAX_FNAME, NULL, 0 );
      _strupr_s( appName, _MAX_FNAME );

      if ( isHelpRequest( argv[ 1 ] ) )
      {
         help( appName );
      }
      else
      {
         cerr << "ERROR: Invalid argument/option - '" << argv[ 1 ] << "'." << endl;
         cerr << "Type \"" << appName << " /?\" for usage." << endl;
         rc = 1;
      }
   }

   // Set ERRORCODE on exit. 
   // 0 is OK, anything else indicates an error occurred
   return rc;
}

int paste()
{
   int rc = 0;
   if ( ::OpenClipboard( NULL ) )
   {
      // Use try...finally to make sure we close the clipboard after use
      __try
      {
         rc = readClipboard();
      }
      __finally
      {
         ::CloseClipboard();
      }
   }
   else
   {
      cerr << "ERROR: Cannot not access clipboard." << endl;
      rc = 2;
   }

   return rc;
}

int readClipboard()
{
   int rc = 0;
   
   UINT format = 0;
   do
   {
      // Get each format in turn
      format = ::EnumClipboardFormats( format );

      // If we are interested in the format, we will try and extract it
      if ( format == CF_TEXT || format == CF_OEMTEXT )
      {
         HANDLE h = ::GetClipboardData( format );
         if ( h != nullptr )
         {
            rc = processData( h );

            // We did something with this format of data, so break out of the loop and return
            break;
         }
      }
   }
   while ( format != 0 && rc == 0 );

   return rc;
}

int processData( const HANDLE& h )
{
   int rc = 0;

   // Use try...finally to make sure we unlock any memory that we lock
   LPVOID data = GlobalLock( h );
   __try
   {
      char* pszText = static_cast<char*>( data );
      if ( pszText != nullptr )
      {
         cout << pszText;
      }
      else
      {
         cerr << "ERROR: Cannot get clipboard contents." << endl;
         rc = 3;
      }
   }
   __finally
   {
      if ( data != nullptr )
      {
         GlobalUnlock( h );
      }
   }

   return rc;
}

/// <summary>
/// Returns whether the provided argument is a request for help
/// </summary>
/// <param name="arg">the argument</param>
/// <returns>true if the argument looks like a request for help</returns>
bool isHelpRequest( const char* arg )
{
   return strcmp( arg, "/?" ) == 0 || strcmp( arg, "-?" ) == 0;
}

/// <summary>
/// Display help information to the output stream, including some example uses
/// </summary>
/// <param name="app">the name of this executable</param>
/// <returns>a return code for the application. In this case, 0</returns>
void help( const char* app )
{
   cout << endl << app << endl;
   cout << endl << "Description:";
   cout << endl << "    Pastes the textual content of the Windows clipboard to the output stream." << endl;
   cout << endl << "Parameter List:";
   cout << endl << "    /?                  Displays this help message." << endl;
   cout << endl << "Examples:";
   cout << endl << "    " << app << "               Writes text from the Windows clipboard to the console." << endl;
   cout << endl << "    " << app << " > README.TXT  Writes text from the Windows clipboard to the README.TXT file." << endl;
}
