/*
    * 9/2024 by Thai-Son Nguyen.
    * Encoder for temperature-humidity data (th-data)
*/

// ************* INTERFACE ****************
#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <cJSON.h>



#define C_OK 0
#define C_ERROR 1

// ************** INTERFACE *************

// encode temperature and humidity data to a JSON string
// @return
// return length of output string.
char* encode_th_data(
    char* device_id, // 48 bits mac address
    double temperature,
    int humidity
);


// ************ IMPLEMENTATION ****************
inline char* encode_th_data(
    char* device_id, // 48 bits mac address
    double temperature,
    int humidity
  
) {
    printf("values: %s, %f, %d\n",device_id, temperature, humidity);
    cJSON *root = cJSON_CreateObject();
    
    cJSON_AddStringToObject(root,"device_id",       device_id);
    cJSON_AddNumberToObject(root,"temparature",		temperature);
    cJSON_AddNumberToObject(root,"humidity",		humidity);

    
    char* output_buffer = cJSON_Print(root) ;
    // printf("str: %s\n",output_buffer);

    cJSON_Delete(root);
    return (char*) output_buffer;
};

#ifdef __cplusplus
}
#endif
#endif // ENCODER_H