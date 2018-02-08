#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


char *GetError(void)
{
    static char buffer[1000];

    memset(buffer, 0, sizeof(buffer));

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  GetLastError( ),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buffer, 
                  sizeof(buffer), 
                  NULL);

    return buffer;
}

BOOL CALLBACK DesktopCallback(LPTSTR lpszDesktop, LPARAM lParam)
{
    printf("       Desktop: %s\n", lpszDesktop); 

    return TRUE;
}

BOOL CALLBACK WindowStationsCallback(LPTSTR lpszWindowStation, LPARAM lParam)
{
    BOOL status;
    HWINSTA hWindowStation;

    printf("\nWindow Station: %s\n", lpszWindowStation); 

    // Open this window station.
    hWindowStation = 
        OpenWindowStation(lpszWindowStation, FALSE, WINSTA_ENUMDESKTOPS);
    if (hWindowStation == NULL)
    {
        fprintf(stderr, "OpenWindowStation: %s", GetError( ));
        return TRUE;
    }

    // Cycle through all the desktops for this window station.
    status = EnumDesktops(hWindowStation, DesktopCallback, 0);
    if (status == FALSE)
    {
        fprintf(stderr, "EnumDesktops: %s", GetError( ));
        return FALSE;
    }

    // Close this window station.
    CloseWindowStation(hWindowStation);

    return TRUE;
}

int main(int argc, char *argv[])
{
    BOOL status;

    // Cycle through all the window stations. 
    status = EnumWindowStations(WindowStationsCallback, 0);
    if (status == FALSE)
    {
        fprintf(stderr, "EnumWindowStations: %s", GetError( ));
        exit(1);
    }

    exit(0);
}
