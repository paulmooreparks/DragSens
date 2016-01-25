/* 
Dragsens: Modify the drag sensitivity for Windows.

Paul M. Parks
paul@parkscomputing.com
http://www.parkscomputing.com/
*/

#include "stdafx.h"

void usage(bool showDescription)
{
   const wchar_t description[] =
      L"Changes the default drag sensitivity.\n"
      L"\n";

   const wchar_t parameters[] =
      L"DRAGSENS [pixels] [/X pixels] [/Y pixels] [/D]\n"
      L"\n"
      L"  pixels  The number of pixels the mouse must travel to initiate\n"
      L"          a drag operation. Providing just a number will change both \n"
      L"          the X axis and the Y axis to the same value.\n"
      L"\n"
      L"  /X      Change the setting for the X axis (horizontal).\n"
      L"\n"
      L"  /Y      Change the setting for the Y axis (vertical).\n"
      L"\n"
      L"  /D      Display the current drag sensitivity settings.\n";

   const wchar_t version[] =
      L"\n"
      L"Version 1.5\n"
      L"Paul M. Parks\n"
      L"paul@parkscomputing.com\n"
      L"http://www.parkscomputing.com/applications/DragSens/\n";

   if (showDescription) {
      std::wcout << description;
   }

   std::wcout << parameters;

   if (showDescription) {
      std::wcout << version;
   }
}

int wmain(int argc, wchar_t* argv[])
{
   if (argc == 1) {
      usage(true);
      return 0;
   }

   if (_wcsicmp(argv[1], L"/?") == 0 || _wcsicmp(argv[1], L"-?") == 0 ) {
      usage(true);
      return 0;
   }

   typedef std::vector<std::wstring> Args;
   Args args;
   int i = 1;

   for (; i < argc; ++i) {
      args.push_back(argv[i]);
   }

   bool displaySettings = false;
   bool setX = false;
   UINT xPixels = 0;
   bool setY = false;
   UINT yPixels = 0;

   std::wstringstream cvt;
   Args::const_iterator it = args.begin();
   UINT numPixels = 0;

   if (cvt << *it && cvt >> numPixels) {
      xPixels = numPixels;
      setX = true;
      yPixels = numPixels;
      setY = true;
      ++it;
   }

   while (it != args.end()) {
      cvt.clear();
      cvt.str(L"");

      std::wstring arg = *it;
      wchar_t c = arg[0];

      if (c == L'/' || c == L'-') {
         c = arg[1];

         if (c == L'?') {
            usage(true);
            return 0;
         }
         else if (c == L'x' || c == L'X') {
            ++it;

            if (cvt << *it && cvt >> xPixels) {
               setX = true;
            }
            else {
               std::wcout << L"Incorrect parameter: " << *it << std::endl << std::endl;
               usage(false);
               return 2;
            }
         }
         else if (c == L'y' || c == L'Y') {
            ++it;

            if (cvt << *it && cvt >> yPixels) {
               setY = true;
            }
            else {
               std::wcout << L"Incorrect parameter: " << *it << std::endl << std::endl;
               usage(false);
               return 2;
            }
         }
         else if (c == L'd' || c == L'D') {
            displaySettings = true;
         }
         else {
            std::wcout << L"Incorrect parameter: " << arg << std::endl << std::endl;
            usage(false);
            return 2;
         }
      }
      else {
         std::wcout << L"Incorrect parameter: " << arg << std::endl << std::endl;
         usage(false);
         return 2;
      }

      ++it;
   }

   if (!(setX || setY || displaySettings)) {
      std::wcout << L"Incorrect parameters" << std::endl << std::endl;
      usage(false);
      return 2;
   }
   
   BOOL success = FALSE;

   if (displaySettings) {
      int oldxPixels = GetSystemMetrics(SM_CXDRAG);
      int oldyPixels = GetSystemMetrics(SM_CYDRAG);

      std::wcout << L"Current settings:" << std::endl;
      std::wcout << L"X=" << oldxPixels << L", Y=" << oldyPixels << std::endl;

      if (setX || setY) {
         std::wcout << std::endl;
         std::wcout << L"New settings:" << std::endl;
      }
   }

   if (setX) {
      success = SystemParametersInfo(SPI_SETDRAGWIDTH, xPixels, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

      if (!success) {
         DWORD error = GetLastError();
         std::wcout << L"Error " << std::hex << error << std::dec << " while setting drag width." << std::endl;
         return 1;
      }

      if (displaySettings) {
         std::wcout << L"X=" << xPixels;

         if (setY) {
            std::wcout << L", ";
         }
         else {
            std::wcout << std::endl;
         }
      }
   }

   if (setY) {
      success = SystemParametersInfo(SPI_SETDRAGHEIGHT, yPixels, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

      if (!success) {
         DWORD error = GetLastError();
         std::wcout << L"Error " << std::hex << error << std::dec << " while setting drag height." << std::endl;
         return 1;
      }

      if (displaySettings) {
         std::wcout << L"Y=" << yPixels << std::endl;
      }
   }

   return 0;
}

