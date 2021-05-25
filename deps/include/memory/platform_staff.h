#pragma once

#pragma once

#ifndef _X64_

#define __THISCALL __thiscall
#define __FASTCALL __fastcall
#define __CDECL __cdecl
#define __STDCALL __stdcall

#else

#define __THISCALL
#define __FASTCALL
#define __CDECL
#define __STDCALL

#endif