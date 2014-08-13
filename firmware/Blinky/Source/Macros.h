#ifndef BLE_MACROS
#define BLE_MACROS

#include "att.h"
#include "gatt_uuid.h"

#define BLE_PRIMARY_SERVICE(permissions, valuePtr) \
  { \
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, \
    permissions, \
    0, \
    valuePtr \
  }

#define BLE_START_CHARACTERISTIC(description, permissions, valuePtr) \
  /* description is unused intentionally, it's for code-readability at the call-site */ \
  { { ATT_BT_UUID_SIZE, characterUUID }, \
    permissions, \
    0, \
    valuePtr \
  }

#define BLE_CHARACTERISTIC_VALUE(permissions, uuid, valuePtr) \
  { \
    { ATT_UUID_SIZE, uuid }, \
    permissions, \
    0, \
    valuePtr \
  }

#define BLE_CHARACTERISTIC_CLIENT_CONFIG(permissions, valuePtr) \
  { \
    { ATT_BT_UUID_SIZE, clientCharCfgUUID }, \
    permissions, \
    0, \
    valuePtr \
  }  

#define BLE_CHARACTERISTIC_USER_DESCRIPTION(permissions, valuePtr) \
  { \
    { ATT_BT_UUID_SIZE, charUserDescUUID }, \
    permissions, \
    0, \
    valuePtr \
  }

#endif /* BLE_MACROS */