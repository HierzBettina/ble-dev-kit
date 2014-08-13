#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "blinkyProfile.h"

#define SERVAPP_NUM_ATTR_SUPPORTED        14

// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 blinkyProfileServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(BLINKYPROFILE_SERV_UUID), HI_UINT16(BLINKYPROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 blinkyProfilechar1UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(BLINKYPROFILE_CHAR1_UUID), HI_UINT16(BLINKYPROFILE_CHAR1_UUID)
};

// Characteristic 2 UUID: 0xFFF2
CONST uint8 blinkyProfilechar2UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(BLINKYPROFILE_CHAR2_UUID), HI_UINT16(BLINKYPROFILE_CHAR2_UUID)
};

// Characteristic 3 UUID: 0xFFF3
CONST uint8 blinkyProfilechar3UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(BLINKYPROFILE_CHAR3_UUID), HI_UINT16(BLINKYPROFILE_CHAR3_UUID)
};

// Characteristic 4 UUID: 0xFFF4
CONST uint8 blinkyProfilechar4UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(BLINKYPROFILE_CHAR4_UUID), HI_UINT16(BLINKYPROFILE_CHAR4_UUID)
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
static uint8 blinkyProfileChar1UserDesp[17] = "Characteristic 1\0";


// Simple Profile Characteristic 2 Properties
static uint8 blinkyProfileChar2Props = GATT_PROP_READ;
static uint8 blinkyProfileChar2 = 0;
static uint8 blinkyProfileChar2UserDesp[17] = "Characteristic 2\0";


static uint8 blinkyProfileChar3Props = GATT_PROP_WRITE;
static uint8 blinkyProfileChar3 = 0;
static uint8 blinkyProfileChar3UserDesp[17] = "Characteristic 3\0";

// Simple Profile Characteristic 4 Properties
static uint8 blinkyProfileChar4Props = GATT_PROP_NOTIFY;
static uint8 blinkyProfileChar4 = 0;
static gattCharCfg_t blinkyProfileChar4Config[GATT_MAX_NUM_CONN];
static uint8 blinkyProfileChar4UserDesp[17] = "Characteristcc 4\0";


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t blinkyProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&blinkyProfileService            /* pValue */
  },

    // Characteristic 1 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &blinkyProfileChar1Props 
    },

      // Characteristic Value 1
      { 
        { ATT_BT_UUID_SIZE, blinkyProfilechar1UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        &blinkyProfileChar1 
      },

      // Characteristic 1 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        blinkyProfileChar1UserDesp 
      },      

    // Characteristic 2 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &blinkyProfileChar2Props 
    },

      // Characteristic Value 2
      { 
        { ATT_BT_UUID_SIZE, blinkyProfilechar2UUID },
        GATT_PERMIT_READ, 
        0, 
        &blinkyProfileChar2 
      },

      // Characteristic 2 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        blinkyProfileChar2UserDesp 
      },           
      
    // Characteristic 3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &blinkyProfileChar3Props 
    },

      // Characteristic Value 3
      { 
        { ATT_BT_UUID_SIZE, blinkyProfilechar3UUID },
        GATT_PERMIT_WRITE, 
        0, 
        &blinkyProfileChar3 
      },

      // Characteristic 3 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        blinkyProfileChar3UserDesp 
      },

    // Characteristic 4 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &blinkyProfileChar4Props 
    },

      // Characteristic Value 4
      { 
        { ATT_BT_UUID_SIZE, blinkyProfilechar4UUID },
        0, 
        0, 
        &blinkyProfileChar4 
      },

      // Characteristic 4 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)blinkyProfileChar4Config 
      },
      
      // Characteristic 4 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        blinkyProfileChar4UserDesp 
      },


};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 blinkyProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t blinkyProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void blinkyProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


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

  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( blinkyProfile_HandleConnStatusCB );  
  
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
  

/*********************************************************************
 * @fn      BlinkyProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t BlinkyProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case BLINKYPROFILE_CHAR1:
      if ( len == sizeof ( uint8 ) ) 
      {
        blinkyProfileChar1 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case BLINKYPROFILE_CHAR2:
      if ( len == sizeof ( uint8 ) ) 
      {
        blinkyProfileChar2 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case BLINKYPROFILE_CHAR3:
      if ( len == sizeof ( uint8 ) ) 
      {
        blinkyProfileChar3 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case BLINKYPROFILE_CHAR4:
      if ( len == sizeof ( uint8 ) ) 
      {
        blinkyProfileChar4 = *((uint8*)value);
        
        // See if Notification has been enabled
        GATTServApp_ProcessCharCfg( blinkyProfileChar4Config, &blinkyProfileChar4, FALSE,
                                    blinkyProfileAttrTbl, GATT_NUM_ATTRS( blinkyProfileAttrTbl ),
                                    INVALID_TASK_ID );
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

/*********************************************************************
 * @fn      BlinkyProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t BlinkyProfile_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case BLINKYPROFILE_CHAR1:
      *((uint8*)value) = blinkyProfileChar1;
      break;

    case BLINKYPROFILE_CHAR2:
      *((uint8*)value) = blinkyProfileChar2;
      break;      

    case BLINKYPROFILE_CHAR3:
      *((uint8*)value) = blinkyProfileChar3;
      break;  

    case BLINKYPROFILE_CHAR4:
      *((uint8*)value) = blinkyProfileChar4;
      break;     
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          blinkyProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
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
      case BLINKYPROFILE_CHAR1_UUID:
      case BLINKYPROFILE_CHAR2_UUID:
      case BLINKYPROFILE_CHAR4_UUID:
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

/*********************************************************************
 * @fn      blinkyProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
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
      case BLINKYPROFILE_CHAR1_UUID:
      case BLINKYPROFILE_CHAR3_UUID:

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
            notifyApp = BLINKYPROFILE_CHAR1;        
          }
          else
          {
            notifyApp = BLINKYPROFILE_CHAR3;           
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

/*********************************************************************
 * @fn          blinkyProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void blinkyProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      GATTServApp_InitCharCfg( connHandle, blinkyProfileChar4Config );
    }
  }
}
