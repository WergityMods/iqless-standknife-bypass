#include <cstdio>
#include <intrin.h>
#include <iostream>
#include <Windows.h>

#include "minhook/MinHook.h"

#define MH_SHOW_ERROR( ) printf( "[ERROR] [%s] [%d]\n", __FUNCTION__, __LINE__ )

void show_console( )
{
    AllocConsole( );
    SetConsoleTitleA( "T.ME/WERGITY_MODS" );
    freopen_s( ( FILE** ) stdout, "CONOUT$", "w", stdout );
}

int ( __stdcall* og_inet_pton ) ( int, const char*, void** );
int __stdcall hk_inet_pton( int family, const char* link, void** buffer_ptr )
{
    if ( strstr( link, "92.246.136.224" ) )
    {
        link = "127.0.0.1";
    }

    return og_inet_pton( family, link, buffer_ptr );
}

void ( __stdcall* og_sleep )( DWORD );
void __stdcall hk_sleep( DWORD ms )
{
    if ( ms == 5000 )
    {
        ms = 999999999;
    }

    og_sleep( ms );
}

void core( )
{
    if ( MH_Initialize( ) != MH_OK )
    {
        MH_SHOW_ERROR( );
        return;
    }

    if ( MH_CreateHookApi( L"Ws2_32.dll", "inet_pton", ( void* ) hk_inet_pton, ( void** ) &og_inet_pton ) != MH_OK )
    {
        MH_SHOW_ERROR( );
        return;
    }

    if ( MH_CreateHookApi( L"Kernel32.dll", "Sleep", ( void* ) hk_sleep, ( void** ) &og_sleep ) != MH_OK )
    {
        MH_SHOW_ERROR( );
        return;
    }

    if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
    {
        MH_SHOW_ERROR( );
        return;
    }
}

bool __stdcall DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
    if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        show_console( );
        CreateThread( 0, 0, LPTHREAD_START_ROUTINE( core ), 0, 0, 0 );
    }

    return true;
}