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
  { { 2, characterUUID }, \
    permissions, \
    0, \
    valuePtr \
  }

#define BLE_CHARACTERISTIC_VALUE(permissions, uuid, valuePtr) \
  { \
    { 2, uuid }, \
    permissions, \
    0, \
    valuePtr \
  }

#define BLE_CHARACTERISTIC_CLIENT_CONFIG(permissions, valuePtr) \
  { \
    { 2, clientCharCfgUUID }, \
    permissions, \
    0, \
    valuePtr \
  }  

#define BLE_CHARACTERISTIC_USER_DESCRIPTION(permissions, valuePtr) \
  { \
    { 2, charUserDescUUID }, \
    permissions, \
    0, \
    valuePtr \
  }

#endif /* BLE_MACROS */