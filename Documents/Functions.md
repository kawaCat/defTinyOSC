# Functions Description

## Generate OSC message 

**length, string =  create_OSCMessage_SocketBuffer( addres, format , data )** 

Example:

``` lua
-- double 
TinyOSC.create_OSCMessage_SocketBuffer( "/Test/param01" -- target Address
                                      , "d"             -- format is double
                                      , math.random()   -- send Random Number()
                                      );
-- int32
TinyOSC.create_OSCMessage_SocketBuffer( "/Test/param02" -- target Address
                                      , "i"             -- format is int
                                      , 12345           -- send 
                                      );
-- string
TinyOSC.create_OSCMessage_SocketBuffer( "/Test/strName"        -- target Address
                                      , "s"                    -- format is string
                                      , "this is test String"  -- send 
                                      );

-- midi message
TinyOSC.create_OSCMessage_SocketBuffer( "/Test/midiMsg01"      -- target Address
                                      , "m"                    -- format is string
                                      ,  string.char( "0x90" ) -- first byte
                                      .. string.char( "0x64" ) -- second byte
                                      .. string.char( "0x30" ) -- third byte
                                      .. string.char( "0x00" ) -- 4th byte 
                                      );

-- blob --prototype
TinyOSC.create_OSCMessage_SocketBuffer( "/Test/blob"      -- target Address
                                      , "b"               -- format is string
                                      ,  <<stringBuf>>    -- or userdata ??
                                      ,  <<length>>       -- buf size
                                      );

```

## Parse OSC message 

| functions                                                            |  Description                                                              |
|----------------------------------------------------------------------|---------------------------------------------------------------------------|
| TinyOSC.Message_create()                                             | create TinyOSC object. ( retrun TinyOSC Object)                           |
| TinyOSC.Message_delete ()                                            | delete TinyOSC object.                                                    |
| TinyOSC.Message_parseMessage ( TinyOSC_Object , data , dataLength )  | parse UDP data as OSC message, and store parsed result to TinyOSC_Object. |
| TinyOSC.Message_getAddress ( TinyOSC_Object )                        | get OSC Address ( e.g /test/param1 ) from parsed TinyOSC object.          |
| TinyOSC.Message_getFormat ( TinyOSC_Object )                         |                                                                           |
| TinyOSC.Message_getNextInt32 ( TinyOSC_Object )                      | get Parameters. these function will increament read postion state.        |
| TinyOSC.Message_getNextInt64 ( TinyOSC_Object )                      |                                                                           |
| TinyOSC.Message_getNextTimeTag ( TinyOSC_Object )                    |                                                                           |
| TinyOSC.Message_getNextFloat ( TinyOSC_Object )                      |                                                                           |
| TinyOSC.Message_getNextDouble ( TinyOSC_Object )                     |                                                                           |
| TinyOSC.Message_getNextString ( TinyOSC_Object )                     |                                                                           |
| TinyOSC.Message_getNextMidi ( TinyOSC_Object )                       |                                                                           |
| TinyOSC.Message_getNextBlob ( TinyOSC_Object )                       |                                                                           |
| TinyOSC.Message_getPrintMessage ( TinyOSC_Object m,isCopy)           | this function is dirty check/print message. only use debug.               |
|                                                                      |                                                                           |


