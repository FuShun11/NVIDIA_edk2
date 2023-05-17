/** @file
  APIs for JSON operations.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
 (C) Copyright 2021 Hewlett Packard Enterprise Development LP<BR>
  Copyright (c) 2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef JSON_LIB_H_
#define JSON_LIB_H_

typedef    VOID  *EDKII_JSON_VALUE;
typedef    VOID  *EDKII_JSON_ARRAY;
typedef    VOID  *EDKII_JSON_OBJECT;

///
/// Map to json_int_t in jansson.h
///
typedef    INT64 EDKII_JSON_INT_T;   // #JSON_INTEGER_IS_LONG_LONG is set to 1
                                     // in jansson_Config.h

///
/// Map to the definitions in jansson.h
/// See below URI for the JSON encoding flags reference.
/// https://jansson.readthedocs.io/en/2.13/apiref.html#encoding
///
#define EDKII_JSON_MAX_INDENT  0x1F
#define EDKII_JSON_INDENT(n)  ((n) & EDKII_JSON_MAX_INDENT)

#define EDKII_JSON_COMPACT         0x20
#define EDKII_JSON_ENSURE_ASCII    0x40
#define EDKII_JSON_SORT_KEYS       0x80
#define EDKII_JSON_PRESERVE_ORDER  0x100
#define EDKII_JSON_ENCODE_ANY      0x200
#define EDKII_JSON_ESCAPE_SLASH    0x400
#define EDKII_JSON_REAL_PRECISION(n)  (((n) & 0x1F) << 11)
#define EDKII_JSON_EMBED  0x10000

///
/// Map to the definitions in jansson.h
/// See below URI for the JSON decoding flags reference.
/// https://jansson.readthedocs.io/en/2.13/apiref.html?highlight=json_loadb#decoding
///
#define EDKII_JSON_REJECT_DUPLICATES   0x1
#define EDKII_JSON_DISABLE_EOF_CHECK   0x2
#define EDKII_JSON_DECODE_ANY          0x4
#define EDKII_JSON_DECODE_INT_AS_REAL  0x8
#define EDKII_JSON_ALLOW_NUL           0x10

#define EDKII_JSON_ARRAY_FOREACH(Array, Index, Value) \
  for(Index = 0; \
    Index < JsonArrayCount(Array) && (Value = JsonArrayGetValue(Array, Index)); \
    Index++)

#define EDKII_JSON_OBJECT_FOREACH_SAFE(Object, N, Key, Value)           \
    for (Key = JsonObjectIteratorKey(JsonObjectIterator(Object)),                \
        N = JsonObjectIteratorNext(Object, JsonObjectKeyToIterator(Key));        \
        Key && (Value = JsonObjectIteratorValue(JsonObjectKeyToIterator(Key)));  \
        Key = JsonObjectIteratorKey(N),                                      \
        N = JsonObjectIteratorNext(Object, JsonObjectKeyToIterator(Key)))

///
///  Map to the json_error_t in jansson.h
///
#define EDKII_JSON_ERROR_TEXT_LENGTH    160
#define EDKII_JSON_ERROR_SOURCE_LENGTH  80
typedef struct {
  INTN     Line;
  INTN     Column;
  INTN     Position;
  CHAR8    Source[EDKII_JSON_ERROR_SOURCE_LENGTH];
  CHAR8    Text[EDKII_JSON_ERROR_TEXT_LENGTH];
} EDKII_JSON_ERROR;

///
///  Map to the json_type in jansson.h
///
typedef enum {
  EdkiiJsonTypeObject,
  EdkiiJsonTypeArray,
  EdkiiJsonTypeString,
  EdkiiJsonTypeInteger,
  EdkiiJsonTypeReal,
  EdkiiJsonTypeTrue,
  EdkiiJsonTypeFalse,
  EdkiiJsonTypeNull
} EDKII_JSON_TYPE;

/**
  The function is used to initialize a JSON value which contains a new JSON array,
  or NULL on error. Initially, the array is empty.

  The reference count of this value will be set to 1, and caller needs to cleanup the
  value by calling JsonValueFree().

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @retval      The created JSON value which contains a JSON array or NULL if initial a JSON array
               is failed.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitArray (
  VOID
  );

/**
  The function is used to initialize a JSON value which contains a new JSON object,
  or NULL on error. Initially, the object is empty.

  The reference count of this value will be set to 1, and caller needs to cleanup the
  value by calling JsonValueFree().

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @retval      The created JSON value which contains a JSON object or NULL if initial a JSON object
               is failed.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitObject (
  VOID
  );

/**
  The function is used to initialize a JSON value which contains a new JSON string,
  or NULL on error.

  The input string must be NULL terminated Ascii format, non-Ascii characters will
  be processed as an error. Unicode characters can also be represented by Ascii string
  as the format: \u + 4 hexadecimal digits, like \u3E5A, or \u003F.

  The reference count of this value will be set to 1, and caller needs to cleanup the
  value by calling JsonValueFree().

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @param[in]   String      The Ascii string to initialize to JSON value

  @retval      The created JSON value which contains a JSON string or NULL. Select a
               Getter API for a specific encoding format.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitAsciiString (
  IN    CONST CHAR8  *String
  );

/**
  The function is used to initialize a JSON value which contains a new JSON string,
  or NULL on error.

  The input must be a NULL terminated UCS2 format Unicode string.

  The reference count of this value will be set to 1, and caller needs to cleanup the
  value by calling JsonValueFree().

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @param[in]   String      The Unicode string to initialize to JSON value

  @retval      The created JSON value which contains a JSON string or NULL. Select a
               Getter API for a specific encoding format.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitUnicodeString (
  IN    CHAR16  *String
  );

/**
  The function is used to initialize a JSON value which contains a new JSON integer,
  or NULL on error.

  The reference count of this value will be set to 1, and caller needs to cleanup the
  value by calling JsonValueFree().

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @param[in]   Value       The integer to initialize to JSON value

  @retval      The created JSON value which contains a JSON integer or NULL.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitInteger (
  IN    INT64  Value
  );

/**
  The function is used to initialize a JSON value which contains a new JSON boolean,
  or NULL on error.

  Boolean JSON value is kept as static value, and no need to do any cleanup work.

  @param[in]   Value       The boolean value to initialize.

  @retval      The created JSON value which contains a JSON boolean or NULL.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitBoolean (
  IN    BOOLEAN  Value
  );

/**
  The function is used to initialize a JSON value which contains a new JSON NULL,
  or NULL on error.

  NULL JSON value is kept as static value, and no need to do any cleanup work.

  @retval      The created NULL JSON value.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitNull (
  VOID
  );

/**
  The function is used to initialize a JSON value which contains a TRUE JSON value,
  or NULL on error.

  NULL JSON value is kept as static value, and no need to do any cleanup work.

  @retval      The created JSON TRUE value.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitTrue (
  VOID
  );

/**
  The function is used to initialize a JSON value which contains a FALSE JSON value,
  or NULL on error.

  NULL JSON value is kept as static value, and no need to do any cleanup work.

  @retval      The created JSON FALSE value.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueInitFalse (
  VOID
  );

/**
  The function is used to decrease the reference count of a JSON value by one, and once
  this reference count drops to zero, the value is destroyed and it can no longer be used.
  If this destroyed value is object type or array type, reference counts for all containing
  JSON values will be decreased by 1. Boolean JSON value and NULL JSON value won't be destroyed
  since they are static values kept in memory.

  Reference Count Strategy: BaseJsonLib uses this strategy to track whether a value is still
  in use or not. When a value is created, it's reference count is set to 1. If a reference to a
  value is kept for use, its reference count is incremented, and when the value is no longer
  needed, the reference count is decremented. When the reference count drops to zero, there are
  no references left, and the value can be destroyed.

  The given JSON value maybe NULL and not causing any problem. Just output the debug message
  to inform caller the NULL value is passed in.

  @param[in]   Json             The JSON value to be freed. json_decref may return without any
                                changes if Json is NULL.

**/
VOID
EFIAPI
JsonValueFree (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to create a fresh copy of a JSON value, and all child values are deep
  copied in a recursive fashion. It should be called when this JSON value might be modified
  in later use, but the original still wants to be used in somewhere else.

  Reference counts of the returned root JSON value and all child values will be set to 1, and
  caller needs to cleanup the root value by calling JsonValueFree().

  * Note: Since this function performs a copy from bottom to up, too many calls may cause some
  performance issues, user should avoid unnecessary calls to this function unless it is really
  needed.

  @param[in]   Json             The JSON value to be cloned.

  @retval      Return the cloned JSON value, or NULL on error.

**/
EDKII_JSON_VALUE
EFIAPI
JsonValueClone (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a JSON array.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value contains a JSON array.
  @retval      FALSE            The JSON value doesn't contain a JSON array.

**/
BOOLEAN
EFIAPI
JsonValueIsArray (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a JSON object.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value contains a JSON object.
  @retval      FALSE            The JSON value doesn't contain a JSON object.

**/
BOOLEAN
EFIAPI
JsonValueIsObject (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON Value contains a string, Ascii or
  Unicode format is not differentiated.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value contains a JSON string.
  @retval      FALSE            The JSON value doesn't contain a JSON string.

**/
BOOLEAN
EFIAPI
JsonValueIsString (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a JSON integer.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value is contains JSON integer.
  @retval      FALSE            The JSON value doesn't contain a JSON integer.

**/
BOOLEAN
EFIAPI
JsonValueIsInteger (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a JSON number.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value is contains JSON number.
  @retval      FALSE            The JSON value doesn't contain a JSON number.

**/
BOOLEAN
EFIAPI
JsonValueIsNumber (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a JSON boolean.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value contains a JSON boolean.
  @retval      FALSE            The JSON value doesn't contain a JSON boolean.

**/
BOOLEAN
EFIAPI
JsonValueIsBoolean (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a TRUE value.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value contains a TRUE value.
  @retval      FALSE            The JSON value doesn't contain a TRUE value.

**/
BOOLEAN
EFIAPI
JsonValueIsTrue (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a FALSE value.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value contains a FALSE value.
  @retval      FALSE            The JSON value doesn't contain a FALSE value.

**/
BOOLEAN
EFIAPI
JsonValueIsFalse (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to return if the provided JSON value contains a JSON NULL.

  @param[in]   Json             The provided JSON value.

  @retval      TRUE             The JSON value contains a JSON NULL.
  @retval      FALSE            The JSON value doesn't contain a JSON NULL.

**/
BOOLEAN
EFIAPI
JsonValueIsNull (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to retrieve the associated array in an array type JSON value.

  Any changes to the returned array will impact the original JSON value.

  @param[in]   Json             The provided JSON value.

  @retval      Return the associated array in JSON value or NULL.

**/
EDKII_JSON_ARRAY
EFIAPI
JsonValueGetArray (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to retrieve the associated object in an object type JSON value.

  Any changes to the returned object will impact the original JSON value.

  @param[in]   Json             The provided JSON value.

  @retval      Return the associated object in JSON value or NULL.

**/
EDKII_JSON_OBJECT
EFIAPI
JsonValueGetObject (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to retrieve the associated Ascii string in a string type JSON value.

  Any changes to the returned string will impact the original JSON value.

  @param[in]   Json             The provided JSON value.

  @retval      Return the associated Ascii string in JSON value or NULL.

**/
CONST CHAR8 *
EFIAPI
JsonValueGetAsciiString (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to retrieve the associated Unicode string in a string type JSON value.

  Caller can do any changes to the returned string without any impact to the original JSON
  value, and caller needs to free the returned string using FreePool().

  @param[in]   Json             The provided JSON value.

  @retval      Return the associated Unicode string in JSON value or NULL.

**/
CHAR16 *
EFIAPI
JsonValueGetUnicodeString (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to retrieve the associated integer in a integer type JSON value.

  The input JSON value should not be NULL or contain no JSON Integer, otherwise it will
  ASSERT() and return 0.

  @param[in]   Json             The provided JSON value.

  @retval      Return the associated Integer in JSON value.

**/
INT64
EFIAPI
JsonValueGetInteger (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to retrieve the associated boolean in a boolean type JSON value.

  The input JSON value should not be NULL or contain no JSON boolean, otherwise it will
  ASSERT() and return FALSE.

  @param[in]   Json             The provided JSON value.

  @retval      Return the associated value of JSON boolean.

**/
BOOLEAN
EFIAPI
JsonValueGetBoolean (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to retrieve the associated string in a string type JSON value.

  Any changes to the returned string will impact the original JSON value.

  @param[in]   Json             The provided JSON value.

  @retval      Return the associated Ascii string in JSON value or NULL on errors.

**/
CONST CHAR8 *
EFIAPI
JsonValueGetString (
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to get the number of elements in a JSON object, or 0 if it is NULL or
  not a JSON object.

  @param[in]   JsonObject              The provided JSON object.

  @retval      Return the number of elements in this JSON object or 0.

**/
UINTN
EFIAPI
JsonObjectSize (
  IN    EDKII_JSON_OBJECT  JsonObject
  );

/**
  The function removes all elements from object. Returns 0 on success and -1 if object is not
  a JSON object. The reference count of all removed values are decremented.

  @param[in]   JsonObject              The provided JSON object.

  @retval      EFI_ABORTED            Some error occur and operation aborted.
  @retval      EFI_SUCCESS            JSON value has been appended to the end of the JSON array.

**/
EFI_STATUS
EFIAPI
JsonObjectClear (
  IN    EDKII_JSON_OBJECT  JsonObject
  );

/**
  The function is used to enumerate all keys in a JSON object.

  Caller should be responsible to free the returned key array reference using
  FreePool(). But contained keys are read only and must not be modified or freed.

  @param[in]   JsonObj                The provided JSON object for enumeration.
  @param[out]  KeyCount               The count of keys in this JSON object.

  @retval      Return an array of the enumerated keys in this JSON object or NULL if
               JsonObj is not an JSON object, key count is zero or on other errors.

**/
CHAR8 **
JsonObjectGetKeys (
  IN    EDKII_JSON_OBJECT  JsonObj,
  OUT   UINTN              *KeyCount
  );

/**
  The function is used to get a JSON value corresponding to the input key from a JSON object.

  It only returns a reference to this value and any changes on this value will impact the
  original JSON object. If that is not expected, please call JsonValueClone() to clone it to
  use.

  Input key must be a valid NULL terminated UTF8 encoded string. NULL will be returned when
  Key-Value is not found in this JSON object.

  @param[in]   JsonObj           The provided JSON object.
  @param[in]   Key               The key of the JSON value to be retrieved.

  @retval      Return the corresponding JSON value to key, or NULL on error.

**/
EDKII_JSON_VALUE
EFIAPI
JsonObjectGetValue (
  IN    CONST EDKII_JSON_OBJECT  JsonObj,
  IN    CONST CHAR8              *Key
  );

/**
  The function is used to set a JSON value corresponding to the input key from a JSON object,
  and the reference count of this value will be increased by 1.

  Input key must be a valid NULL terminated UTF8 encoded string. If there already is a value for
  this key, this key will be assigned to the new JSON value. The old JSON value will be removed
  from this object and thus its' reference count will be decreased by 1.

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @param[in]   JsonObj                The provided JSON object.
  @param[in]   Key                    The key of the JSON value to be set.
  @param[in]   Json                   The JSON value to set to this JSON object mapped by key.

  @retval      EFI_ABORTED            Some error occur and operation aborted.
  @retval      EFI_SUCCESS            The JSON value has been set to this JSON object.

**/
EFI_STATUS
EFIAPI
JsonObjectSetValue (
  IN    EDKII_JSON_OBJECT  JsonObj,
  IN    CONST CHAR8        *Key,
  IN    EDKII_JSON_VALUE   Json
  );

/**
  The function is used to get the number of elements in a JSON array. Returns or 0 if JsonArray
  is NULL or not a JSON array.

  @param[in]   JsonArray              The provided JSON array.

  @retval      Return the number of elements in this JSON array or 0.

**/
UINTN
EFIAPI
JsonArrayCount (
  IN    EDKII_JSON_ARRAY  JsonArray
  );

/**
  The function is used to return the JSON value in the array at position index. The valid range
  for this index is from 0 to the return value of JsonArrayCount() minus 1.

  It only returns a reference to this value and any changes on this value will impact the
  original JSON object. If that is not expected, please call JsonValueClone() to clone it to
  use.

  If this array is NULL or not a JSON array, or if index is out of range, NULL will be returned.

  @param[in]   JsonArray         The provided JSON Array.

  @retval      Return the JSON value located in the Index position or
               NULL if JsonArray is not an array or no items in the array.

**/
EDKII_JSON_VALUE
EFIAPI
JsonArrayGetValue (
  IN    EDKII_JSON_ARRAY  JsonArray,
  IN    UINTN             Index
  );

/**
  The function is used to append a JSON value to the end of the JSON array, and grow the size of
  array by 1. The reference count of this value will be increased by 1.

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @param[in]   JsonArray              The provided JSON object.
  @param[in]   Json                   The JSON value to append.

  @retval      EFI_ABORTED            Some error occur and operation aborted.
  @retval      EFI_SUCCESS            JSON value has been appended to the end of the JSON array.

**/
EFI_STATUS
EFIAPI
JsonArrayAppendValue (
  IN    EDKII_JSON_ARRAY  JsonArray,
  IN    EDKII_JSON_VALUE  Json
  );

/**
  The function is used to remove a JSON value at position index, shifting the elements after index
  one position towards the start of the array. The reference count of this value will be decreased
  by 1.

  More details for reference count strategy can refer to the API description for JsonValueFree().

  @param[in]   JsonArray              The provided JSON array.
  @param[in]   Index                  The Index position before removal.

  @retval      EFI_ABORTED            Some error occur and operation aborted.
  @retval      EFI_SUCCESS            The JSON array has been removed at position index.

**/
EFI_STATUS
EFIAPI
JsonArrayRemoveValue (
  IN    EDKII_JSON_ARRAY  JsonArray,
  IN    UINTN             Index
  );

/**
  Dump JSON to a buffer.

  @param[in]   JsonValue       The provided JSON value.
  @param[in]   Flags           The Index position before removal. The value
                               could be the combination of below flags.
                                 - EDKII_JSON_INDENT(n)
                                 - EDKII_JSON_COMPACT
                                 - EDKII_JSON_ENSURE_ASCII
                                 - EDKII_JSON_SORT_KEYS
                                 - EDKII_JSON_PRESERVE_ORDER
                                 - EDKII_JSON_ENCODE_ANY
                                 - EDKII_JSON_ESCAPE_SLASH
                                 - EDKII_JSON_REAL_PRECISION(n)
                                 - EDKII_JSON_EMBED
                               See below URI for the JSON encoding flags reference.
                               https://jansson.readthedocs.io/en/2.13/apiref.html#encoding

  @retval      CHAR8 *         Dump fail if NULL returned, otherwise the buffer
                               contain JSON payload in ASCII string. The return
                               value must be freed by the caller FreePool().
**/
CHAR8 *
EFIAPI
JsonDumpString (
  IN    EDKII_JSON_VALUE  JsonValue,
  IN    UINTN             Flags
  );

/**
  Convert a string to JSON object.
  The function is used to convert a NULL terminated CHAR8 string to a JSON
  value. Only object and array represented strings can be converted successfully,
  since they are the only valid root values of a JSON text for UEFI usage.

  Real number and number with exponent part are not supported by UEFI.

  Caller needs to cleanup the root value by calling JsonValueFree().

  @param[in]   String        The NULL terminated CHAR8 string to convert.
  @param[in]   Flags         Flags for loading JSON string.
  @param[in]   Error         Returned error status.

  @retval      Array JSON value or object JSON value, or NULL when any error occurs.

**/
EDKII_JSON_VALUE
EFIAPI
JsonLoadString (
  IN    CONST CHAR8       *String,
  IN    UINT64            Flags,
  IN    EDKII_JSON_ERROR  *Error
  );

/**
  Load JSON from a buffer.

  @param[in]   Buffer        Buffier to the JSON payload
  @param[in]   BufferLen     Length of the buffer
  @param[in]   Flags         Flag of loading JSON buffer, the value
                             could be the combination of below flags.
                               - EDKII_JSON_REJECT_DUPLICATES
                               - EDKII_JSON_DISABLE_EOF_CHECK
                               - EDKII_JSON_DECODE_ANY
                               - EDKII_JSON_DECODE_INT_AS_REAL
                               - EDKII_JSON_ALLOW_NUL
                             See below URI for the JSON encoding flags reference.
                             https://jansson.readthedocs.io/en/2.13/apiref.html?highlight=json_loadb#decoding

  @param[in,out]   Error     Pointer EDKII_JSON_ERROR structure

  @retval      EDKII_JSON_VALUE  NULL means fail to load JSON payload.
**/
EDKII_JSON_VALUE
EFIAPI
JsonLoadBuffer (
  IN    CONST CHAR8        *Buffer,
  IN    UINTN              BufferLen,
  IN    UINTN              Flags,
  IN OUT EDKII_JSON_ERROR  *Error
  );

/**
  The reference count is used to track whether a value is still in use or not.
  When a value is created, it's reference count is set to 1.
  when the value is no longer needed, the reference count is decremented.
  When the reference count drops to zero, there are no references left and the
  value can be destroyed.

  This function decrement the reference count of EDKII_JSON_VALUE. As soon as
  a call to json_decref() drops the reference count to zero, the value is
  destroyed and it can no longer be used.

  @param[in]   JsonValue      JSON value
**/
VOID
EFIAPI
JsonDecreaseReference (
  IN EDKII_JSON_VALUE  JsonValue
  );

/**
  The reference count is used to track whether a value is still in use or not.
  When a value is created, it's reference count is set to 1.
  If a reference to a value is kept (e.g. a value is stored somewhere for later use),
  its reference count is incremented.

  This function increment the reference count of json if it's not NULL.
  Returns EDKII_JSON_VALUE.

  @param[in]   JsonValue      JSON value
  @retval      EDKII_JSON_VALUE of itself
**/
EDKII_JSON_VALUE
EFIAPI
JsonIncreaseReference (
  IN EDKII_JSON_VALUE  JsonValue
  );

/**
  Returns an opaque iterator which can be used to iterate over all key-value pairs
  in object, or NULL if object is empty

  @param[in]   JsonValue      JSON value
**/
VOID *
EFIAPI
JsonObjectIterator (
  IN EDKII_JSON_VALUE  JsonValue
  );

/**
  Extract the associated value from iterator.

  @param[in]   Iterator   Iterator pointer
**/
EDKII_JSON_VALUE
EFIAPI
JsonObjectIteratorValue (
  IN VOID  *Iterator
  );

/**
  Returns an iterator pointing to the next key-value pair in object after iter,
  or NULL if the whole object has been iterated through.

  @param[in]   JsonValue  JSON value
  @param[in]   Iterator   Iterator pointer
  @retval      Iterator pointer
**/
VOID *
EFIAPI
JsonObjectIteratorNext (
  IN EDKII_JSON_VALUE  JsonValue,
  IN VOID              *Iterator
  );

/**
  Returns the key of iterator pointing

  @param[in]   Iterator   Iterator pointer
  @retval      Key
**/
CHAR8 *
EFIAPI
JsonObjectIteratorKey (
  IN VOID  *Iterator
  );

/**
  Returns the pointer of iterator by key.

  @param[in]   Key   The key of interator pointer.
  @retval      Pointer to interator
**/
VOID *
EFIAPI
JsonObjectKeyToIterator (
  IN CHAR8  *Key
  );

/**
  Returns the json type of this json value

  @param[in]   JsonValue  JSON value
  @retval      JSON type returned
**/
EDKII_JSON_TYPE
EFIAPI
JsonGetType (
  IN EDKII_JSON_VALUE  JsonValue
  );

#endif
