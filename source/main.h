
#ifndef MAIN_H
#define MAIN_H

#include <wx/wx.h>


extern wxApp *appHandle;


// main class
class CMain: public wxApp
{
private:
public:
 
 virtual bool OnInit();
 virtual int OnExit();
 
};


#endif
