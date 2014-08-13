#ifndef SIMPLEGATTPROFILE_H
#define SIMPLEGATTPROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

// Profile Parameters
#define BLINKYPROFILE_CHAR1                   0  // RW uint8 - Profile Characteristic 1 value 
#define BLINKYPROFILE_CHAR2                   1  // RW uint8 - Profile Characteristic 2 value
#define BLINKYPROFILE_CHAR3                   2  // RW uint8 - Profile Characteristic 3 value
#define BLINKYPROFILE_CHAR4                   3  // RW uint8 - Profile Characteristic 4 value
  
// Simple Profile Service UUID
#define BLINKYPROFILE_SERV_UUID               0xFFE0
    
// Key Pressed UUID
#define BLINKYPROFILE_CHAR1_UUID            0xFFF1
#define BLINKYPROFILE_CHAR2_UUID            0xFFF2
#define BLINKYPROFILE_CHAR3_UUID            0xFFF3
#define BLINKYPROFILE_CHAR4_UUID            0xFFF4
  
// Simple Keys Profile Services bit fields
#define BLINKYPROFILE_SERVICE               0x00000001

// Callback when a characteristic value has changed
typedef void (*blinkyProfileChange_t)( uint8 paramID );

typedef struct
{
  blinkyProfileChange_t        pfnSimpleProfileChange;  // Called when characteristic value changes
} blinkyProfileCBs_t;

    
extern bStatus_t BlinkyProfile_AddService( uint32 services );
extern bStatus_t BlinkyProfile_RegisterAppCBs( blinkyProfileCBs_t *appCallbacks );
extern bStatus_t BlinkyProfile_SetParameter( uint8 param, uint8 len, void *value );
extern bStatus_t BlinkyProfile_GetParameter( uint8 param, void *value );


#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
