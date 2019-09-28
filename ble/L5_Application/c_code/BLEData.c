#include <stdint.h>
#include <stdbool.h>
#include "c_uart2.h"
#include "c_uart3.h"
#include "stdio.h"
#include "string.h"
#include "can.h"
#include "BLEData.h"
#include "c_io.h"
#include "assert.h"


static int i = 0;
static int j =0;
bool Lat = false;
bool Lon = false;
char Latitude[20]={0};
char Longitude[20]={0};

bool data_recieved= false;
APP_DATA_t BLE_data = {0};
//APP_DATA_LAT_LONG_t gps_destination={0};
APP_DATA_LON_t gps_longitude={0};
APP_DATA_LAT_t gps_latitude={0};

void parse_data()
{

    char coord[100]={0};

    getstring(coord,100,0);
    if(coord[0]==1)
    {
        printf("\nStart");
        BLE_data.START_CAR=1;
        c_LED_display_set_number(1);
        memset(coord,0,sizeof(coord));
    }
    else if(coord[0]==2)
    {
        printf("\nStop");
        BLE_data.START_CAR=2;
        c_LED_display_set_number(0);
        memset(coord,0,sizeof(coord));
    }
    else if(strlen(coord))
    {
        while(coord[i] != ',')
        {
                Latitude[i]=coord[i];
                i++;
        }
       Latitude[i]='\0';
        i++;
        while(coord[i] != '@' )
        {
            Longitude[j]=coord[i];
            i++;
            j++;
        }
        Longitude[j]='\0';
        i=0;
        j=0;
        float LAT1=atof(Latitude);
        float LON1=atof(Longitude);
        printf("\nLatitude: %.15f",LAT1);
        printf("\nLongitude: %.15f",LON1);
        gps_latitude.DEST_LAT=LAT1;
        //gps_destination.DEST_LAT=LAT1;

        //printf("\nbleLatitude: %.15f",gps_destination.DEST_LAT);
        gps_longitude.DEST_LONG=LON1;
        //gps_destination.DEST_LONG=LON1;
        //printf("\nbleLong: %.15f",gps_destination.DEST_LONG);

    }

    dbc_encode_and_send_APP_DATA(&BLE_data);
    dbc_encode_and_send_APP_DATA_LAT(&gps_latitude);
    dbc_encode_and_send_APP_DATA_LON(&gps_longitude);
    //dbc_encode_and_send_APP_DATA_LAT_LONG(&gps_destination);

}

