#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "blinkyProfile.h"
#include "Macros.h"

#define SERVAPP_NUM_ATTR_SUPPORTED        10

// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 blinkyProfileServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(BLINKYPROFILE_SERV_UUID), HI_UINT16(BLINKYPROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 blinkyProfileChar1UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(BLINKYPROFILE_ON_UUID), HI_UINT16(BLINKYPROFILE_ON_UUID)
};


static blinkyProfileCBs_t *blinkyProfile_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t blinkyProfileService = { ATT_BT_UUID_SIZE, blinkyProfileServUUID };

// Simple Profile Characteristic 1 Properties
static uint8 blinkyProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 blinkyProfileChar1 = 0;
static uint8 blinkyProfileChar1UserDescription[17] = "Characteristic 1\0";



/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t blinkyProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  BLE_PRIMARY_SERVICE(GATT_PERMIT_READ, (uint8 *)&blinkyProfileService),
 
    BLE_START_CHARACTERISTIC("On", GATT_PERMIT_READ, &blinkyProfileChar1Props),
      BLE_CHARACTERISTIC_VALUE(GATT_PERMIT_READ | GATT_PERMIT_WRITE, blinkyProfileChar1UUID, &blinkyProfileChar1),
      BLE_CHARACTERISTIC_USER_DESCRIPTION(GATT_PERMIT_READ, blinkyProfileChar1UserDescription),     

};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 blinkyProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t blinkyProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );



/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t blinkyProfileCBs =
{
  blinkyProfile_ReadAttrCB,  // Read callback function pointer
  blinkyProfile_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

bStatus_t BlinkyProfile_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  // Initialize Client Characteristic Configuration attributes
  //GATTServApp_InitCharCfg( INVALID_CONNHANDLE, blinkyProfileChar4Config );
  
  
  if ( services & BLINKYPROFILE_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( blinkyProfileAttrTbl, 
                                          GATT_NUM_ATTRS( blinkyProfileAttrTbl ),
                                          &blinkyProfileCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      BlinkyProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t BlinkyProfile_RegisterAppCBs( blinkyProfileCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    blinkyProfile_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}

bStatus_t BlinkyProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case BLINKYPROFILE_ON:
      if ( len == sizeof ( uint8 ) ) 
      {
        blinkyProfileChar1 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

bStatus_t BlinkyProfile_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case BLINKYPROFILE_ON:
      *((uint8*)value) = blinkyProfileChar1;
      break;  
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

static uint8 blinkyProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads

      // characteristics 1 and 2 have read permissions
      // characteritisc 3 does not have read permissions; therefore it is not
      //   included here
      // characteristic 4 does not have read permissions, but because it
      //   can be sent as a notification, it is included here
      case BLINKYPROFILE_ON_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;
      default:
        // Should never get here! (characteristics 3 and 4 do not have read permissions)
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }

  return ( status );
}


static bStatus_t blinkyProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset )
{
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      case BLINKYPROFILE_ON_UUID:

        //Validate the value
        // Make sure it's not a blob oper
        if ( offset == 0 )
        {
          if ( len != 1 )
          {
            status = ATT_ERR_INVALID_VALUE_SIZE;
          }
        }
        else
        {
          status = ATT_ERR_ATTR_NOT_LONG;
        }
        
        //Write the value
        if ( status == SUCCESS )
        {
          uint8 *pCurValue = (uint8 *)pAttr->pValue;        
          *pCurValue = pValue[0];

          if( pAttr->pValue == &blinkyProfileChar1 )
          {
            notifyApp = BLINKYPROFILE_ON;        
          }
        }
             
        break;

      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    status = ATT_ERR_INVALID_HANDLE;
  }

  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && blinkyProfile_AppCBs && blinkyProfile_AppCBs->pfnSimpleProfileChange )
  {
    blinkyProfile_AppCBs->pfnSimpleProfileChange( notifyApp );  
  }
  
  return ( status );
}
