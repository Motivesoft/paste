#include <iostream>

#include <windows.h>

using namespace std;

int main(int argc, char* argv[])
{
    int rc = 0;
    
    if (argc == 1)
    {
        if (::OpenClipboard(NULL))
        {
            __try
            {
                UINT format = 0;
                do
                {
                    format = ::EnumClipboardFormats(format);
                    if (format == CF_TEXT || format == CF_OEMTEXT)
                    {
                        HANDLE h = ::GetClipboardData(format);
                        if (h != nullptr)
                        {
                            __try
                            {
                                char* pszText = static_cast<char*>(GlobalLock(h));
                                if (pszText != nullptr)
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
                                GlobalUnlock(h);
                            }

                            break;
                        }
                    }
                } while (format != 0 && rc == 0);
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
    }
    else
    {
        // We're going to be showing an message to the user. Reference the executable name when we do this for usage examples
        size_t commandLen = strlen(argv[0]) + 1;
        char* command = new char[commandLen];
        strcpy_s(command, commandLen, argv[0]);
        _strupr_s(command, commandLen);

        if (strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "-?") == 0)
        {
            cout << endl << command << endl;
            cout << endl << "Description:";
            cout << endl << "    Pastes the textual content of the Windows clipboard to the output stream." << endl;
            cout << endl << "Parameter List:";
            cout << endl << "    /?                  Display this help message." << endl;
            cout << endl << "Examples:";
            cout << endl << "    " << command << "               Writes text from the Windows clipboard to the console." << endl;
            cout << endl << "    " << command << " > README.TXT  Writes text from the Windows clipboard to the README.TXT file." << endl;
        }
        else
        {
            cerr << "ERROR: Invalid argument/option - '" << argv[1] << "'." << endl;
            cerr << "Type \"" << command << " /?\" for usage." << endl;
            rc = 1;
        }

        delete[] command;
    }

    return rc;
}