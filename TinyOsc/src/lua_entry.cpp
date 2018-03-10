/*=============================================================================
Copyright (c) 2018, Shinichi kawasaki 

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
=============================================================================*/

//=============================================================================
#if      defined (DM_PLATFORM_WINDOWS) \
      || defined( DM_PLATFORM_OSX )    \
      || defined( DM_PLATFORM_IOS )    \
      || defined( DM_PLATFORM_ANDROID )\
      || defined( DM_PLATFORM_LINUX )
    #define BUILD_AS_DEFOLD_EXT 1
#endif 
//=============================================================================
#ifndef BUILD_AS_DEFOLD_EXT
//=============================================================================
    #ifdef __cplusplus
    extern "C" {
    #endif
    #include "lua.h"
    #include "lauxlib.h"
    #ifdef __cplusplus
    }
    #endif
//=============================================================================
#else
    // include the Defold SDK
    #include <dmsdk/sdk.h>
    #include <stdlib.h>
#endif
//=============================================================================

//=============================================================================
#include "tinyosc.h"
#define MODULE_NAME TinyOSC
#define MODULE_NAME_STR "TinyOSC"
#define LUE_ENTRYPOINT_FUNC 
//=============================================================================
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
//=============================================================================

//=============================================================================
void _errorMSG( lua_State* L, const char* msg)
{
    luaL_error( L, msg );
}
//=============================================================================


// #include <memory>
//=============================================================================
static std::vector < tosc_message* > G_MSG_Vector;
static std::vector < tosc_bundle*  > G_BUNDLE_Vector;
//=============================================================================
tosc_message* _checkUserData_AsTOSC_Message( void* checkTargetPtr )
{
    tosc_message* out= nullptr;
    //=========================================================================
    auto findIt = std::find( G_MSG_Vector.begin(), G_MSG_Vector.end() ,(tosc_message*) checkTargetPtr);
    if ( findIt != G_MSG_Vector.end() )
       out= (tosc_message*)checkTargetPtr;
    //=========================================================================
    return out;
    //=========================================================================
}
//=============================================================================
tosc_bundle* _checkUserData_AsTOSC_Bundle( void* checkTargetPtr )
{
    tosc_bundle* out= nullptr;
    //=========================================================================
    auto findIt = std::find( G_BUNDLE_Vector.begin(), G_BUNDLE_Vector.end() ,(tosc_bundle*) checkTargetPtr);
    if ( findIt != G_BUNDLE_Vector.end() )
       out= (tosc_bundle*)checkTargetPtr;
    //=========================================================================
    return out;
    //=========================================================================
}
//=============================================================================


//=============================================================================
void clearAllStored_OSG_MessageInstance()
{
    //=========================================================================
    for ( auto i : G_MSG_Vector)
    {
        delete i; //delete raw pointer
    }
    //=========================================================================
    G_MSG_Vector.clear();
    //=========================================================================
}
//=============================================================================
void cleareAllStored_OSC_BundleINstance()
{
    //=========================================================================
    for ( auto i : G_BUNDLE_Vector)
    {
        delete i; //delete raw pointer
    }
    //=========================================================================
    G_BUNDLE_Vector.clear();
    //=========================================================================
}
//=============================================================================

//=============================================================================
int TOSC_Message_create          ( lua_State *L ) 
{
    auto ptr = new tosc_message();
    //=========================================================================
    G_MSG_Vector.push_back( ptr );
    //=========================================================================
    lua_pushlightuserdata( L, (void*) ptr);
    return 1; // count of return values.
    //=========================================================================
}
//=============================================================================
int TOSC_Message_delete          ( lua_State *L )
{
    if ( tosc_message* data = static_cast<tosc_message*>(lua_touserdata( L, 1 )) )
    {
        auto findIt = std::find( G_MSG_Vector.begin(), G_MSG_Vector.end() , data);
        if ( findIt != G_MSG_Vector.end() )
        {
            G_MSG_Vector.erase( findIt );
        }
        //=====================================================================
        delete data;
        //=====================================================================
    };
    //=========================================================================
    return 0;
}
//=============================================================================
// optional 
//=============================================================================
int TOSC_Message_getFormat_B     ( lua_State *L )
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushstring( L, data->format );
    else 
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getMarker_B     ( lua_State *L )
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushstring( L, data->marker );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getBuffer_B     ( lua_State *L ) // return 2 ,buffer,buffer length
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
    {
        lua_pushinteger( L, data->len );
        lua_pushlstring( L, data->buffer, (size_t)data->len );
        //=====================================================================
    }
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 2;
}
//=============================================================================
// Message  getter function 
//=============================================================================
int TOSC_Message_getAddress      ( lua_State *L ) // return 1 , string
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushstring( L, tosc_getAddress( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getFormat       ( lua_State *L ) // return 1 , string
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushstring( L, tosc_getFormat( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getLength       ( lua_State *L ) // return 1 , string
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushinteger( L, tosc_getLength( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getNextInt32    ( lua_State *L ) // return 1 , string
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushinteger( L, tosc_getNextInt32( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getNextInt64    ( lua_State *L ) // return 1 , string
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushinteger( L, (int)tosc_getNextInt64( data ) ); // !!
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}   
//=============================================================================
int TOSC_Message_getNextTimeTag  ( lua_State *L ) // return 1 , number
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushnumber( L, (double)tosc_getNextTimetag( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getNextFloat    ( lua_State *L ) // return 1 , number
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushnumber( L, tosc_getNextFloat( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getNextDouble   ( lua_State *L ) // return 1 , number
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushnumber( L, tosc_getNextDouble( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getNextString   ( lua_State *L ) // return 1 , string
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        lua_pushstring( L, tosc_getNextString( data ) );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Message_getNextMidi     ( lua_State *L ) // return 2 , string: midibuffer, number: byteSize
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
    {
        lua_pushinteger( L, 4  );                                 // return arg -1 : size 
        auto m = tosc_getNextMidi( data );
        //=====================================================================
        lua_pushinteger( L, m[0] );                               // retrun arg -2 : 1st byte      
        lua_pushinteger( L, m[1] );                               // retrun arg -3 : 2nd byte      
        lua_pushinteger( L, m[2] );                               // retrun arg -4 : 3rd byte      
        lua_pushinteger( L, m[3] );                               // retrun arg -5 : 4th byte                                             
        //=====================================================================
        char buff[1024];
        snprintf      ( buff, sizeof( buff ), "0x%02X,0x%02X,0x%02X,0x%02X", m[0], m[1], m[2], m[3] );
        lua_pushstring( L   , buff ); // arg -2 : midi byte       // return arg 6
        //=====================================================================
    }
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 6;
}
//=============================================================================
int TOST_Message_getNextBlob     ( lua_State *L )
{
    //=========================================================================
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
    {
        char* buffer;
        //=====================================================================
        int len = 0;
        //=====================================================================
        tosc_getNextBlob( data, (const char**)&buffer, &len );
        //=====================================================================
        lua_pushinteger( L, len );         // arg:-1
        lua_pushlstring( L, buffer, len ); // arg:-2
        //=====================================================================
    }
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 2;
}
//=============================================================================
int TOST_Message_getPrintMessage ( lua_State *L )
{
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
    {
        //=====================================================================
        tosc_message temp = *data; //copy instance
        if ( lua_toboolean( L, 2 ) == true )
            data = &temp;
        //=====================================================================

        //=====================================================================
        std::string out;
        //=====================================================================
        out += " byte: " + std::to_string( data->len ) + "\n";
        out += " address: " + std::string( tosc_getAddress( data ) ) + "\n";
        out += " format: " + std::string( tosc_getFormat( data ) ) + "\n";
        //=====================================================================
        auto sprintfStr = []( const char* sp, void* target )
        {
            char buff[100];
            snprintf( buff, sizeof( buff ), sp, target );
            std::string buffAsStdStr = buff;
            return buffAsStdStr;
        };
        //=====================================================================
        char buff[1000];
        //=====================================================================
        for ( int i = 0; data->format[i] != '\0'; i++ )
        {
            switch ( data->format[i] )
            {
            case 'b': {
                //=============================================================
                const char *b = NULL; // will point to binary data
                int n = 0; // takes the length of the blob
                tosc_getNextBlob( data, &b, &n );
                //=============================================================
                snprintf( buff, sizeof( buff ), "size [ %i ]", n ); // print length of blob
                out += buff;
                //=============================================================
                // for ( int j = 0; j < n; ++j ) printf( "%02X", b[j] & 0xFF ); // print blob bytes
                break;
            }
            case 'm': {
                //=============================================================
                unsigned char *m = tosc_getNextMidi( data );
                snprintf( buff, sizeof( buff ), "[1byte->4byte] : 0x%02X-0x%02X-0x%02X-0x%02X", m[0], m[1], m[2], m[3] );
                std::string buffAsStdStr = buff;
                //=============================================================
                out += buffAsStdStr;
                break;
            }
            case 'f': snprintf( buff, sizeof( buff ), "'f': %f",   tosc_getNextFloat  ( data ) ); out += buff; break;
            case 'd': snprintf( buff, sizeof( buff ), "'d': %lf",  tosc_getNextDouble ( data ) ); out += buff; break;
            case 'i': snprintf( buff, sizeof( buff ), "'i': %d",   tosc_getNextInt32  ( data ) ); out += buff; break;
            case 'h': snprintf( buff, sizeof( buff ), "'h': %lld", tosc_getNextInt64  ( data ) ); out += buff; break;
            case 't': snprintf( buff, sizeof( buff ), "'t': %lld", tosc_getNextTimetag( data ) ); out += buff; break;
            case 's': snprintf( buff, sizeof( buff ), "'s': %s",   tosc_getNextString ( data ) ); out += buff; break;
            case 'F': out += "'F': false"; break;
            case 'I': out += "'I': inf";   break;
            case 'N': out += "'N': nil";   break;
            case 'T': out += "'T': true";  break;
            default: snprintf( buff, sizeof( buff ), " Unknown format: '%c'", data->format[i] ); out += buff; break;
            }
        }
        //=====================================================================
        lua_pushstring( L, out.c_str() );
    }
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 1;
}
//=============================================================================
// create socket buffer for send to.
//=============================================================================
int Create_OSC_MessageBuffer_1   ( lua_State* l)
{
    //=========================================================================
    const char*  address = lua_tostring( l, 1 );
    const char*  format  = lua_tostring( l, 2 );
    //=========================================================================
    void* value = nullptr;
    //=========================================================================
    int len = 1024;
    char buf[1024];
    //=========================================================================

    //=========================================================================
    if (  format[0] == 'm' 
       || format[0] == 's' 
       )
    {
        len = tosc_writeMessage( buf
                               , len
                               , address  
                               , format   
                               , luaL_checkstring( l, 3 ));
    }
    else if( format[0] == 'b')
    {
        int blobSize = luaL_checknumber(l,4);
        //=====================================================================
        len = tosc_writeMessage( buf
                               , len
                               , address  
                               , format   
                               , (int) blobSize
                               , luaL_checkstring( l, 3));

    }
    else if (  format[0] == 'F'
            || format[0] == 'N' )
    {
        len = tosc_writeMessage( buf
                               , len
                               , address 
                               , format  
                               , 0 );
    }
    else if ( format[0] == 'T' )
    {
        len = tosc_writeMessage( buf
                               , len
                               , address 
                               , format  
                               , 1 );
    }
    else
    {
        len = tosc_writeMessage( buf
                               , len
                               , address 
                               , format  
                               , luaL_checknumber( l, 3 ) );
    }
    //=========================================================================
    lua_pushinteger ( l, len );   // -1: length
    lua_pushlstring ( l,buf,len); // -2: osc message buf 
    //=========================================================================
    return 2;
}
//=============================================================================
// parse from socket buffer.
//=============================================================================
int TOST_Message_parseMessage    ( lua_State *L )
{
    //=========================================================================
    int bufferLen       = luaL_checkint(L, 3);// 3rd arg
    size_t bufferLen_t  = bufferLen;
    const char* buffer  = luaL_checklstring (L, 2, &bufferLen_t);
    //=========================================================================
   
    //=========================================================================
    if ( tosc_message* data = _checkUserData_AsTOSC_Message( lua_touserdata( L, 1 )) )
        tosc_parseMessage( data, (char*)buffer, bufferLen );
    else
    {
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Message Class." );
        lua_pushnil(L);
        return 1; // return argument number of counts is 1
    }
    //=========================================================================
    return 0;
}
//=============================================================================

//=============================================================================
// Bundle 
//=============================================================================
int TOSC_Bundle_create           ( lua_State *L) 
{
    auto out = new tosc_bundle();
    //=========================================================================
    G_BUNDLE_Vector.push_back( out);
    lua_pushlightuserdata( L, (void*) out );
    //=========================================================================
    return 1; // count of return values.
    //=========================================================================
}
//=============================================================================
int TOSC_Bundle_delete           ( lua_State *L)
{
    //=========================================================================
    if( tosc_bundle* data = static_cast<tosc_bundle*>( lua_touserdata( L, 1 ) ) )
    {
        auto findIt = std::find( G_BUNDLE_Vector.begin(), G_BUNDLE_Vector.end() , data);
        if ( findIt != G_BUNDLE_Vector.end() )
        {
            G_BUNDLE_Vector.erase( findIt );
        }
        //=====================================================================
        delete data;
    }
    //=========================================================================
    return 0;
}
//=============================================================================
// Getter section
//=============================================================================
int TOSC_isBundle                ( lua_State *L)
{
    auto buffer = luaL_checklstring( L, 1, (size_t*)8 ); // check first  eight byte
    //=========================================================================
    auto isBundle = tosc_isBundle( buffer );
    //=========================================================================
    lua_pushboolean(L, isBundle);
    //=========================================================================
    return 1;
}
//=============================================================================
int TOSC_Bundle_getTimeTag       ( lua_State *L)
{
    if ( tosc_bundle* data =_checkUserData_AsTOSC_Bundle(lua_touserdata( L, 1 )) )
    {
        lua_pushnumber( L, (lua_Number)tosc_getTimetag( data ) );
        return 1;
    }
    else
    {
        lua_pushnil(L);
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Bundle Class." );
        return 1;
    }
}
//=============================================================================
int TOSC_Bundle_getBundleLength  ( lua_State *L)
{
    if ( tosc_bundle* data =_checkUserData_AsTOSC_Bundle(lua_touserdata( L, 1 )) )
    {
        lua_pushnumber( L, tosc_getBundleLength( data ) );
        return 1;
    }
    else
    {
        lua_pushnil(L);
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Bundle Class." );
        return 1;
    }
}
//=============================================================================
int TOSC_Bundle_getNextMessage   ( lua_State *L)
{
    bool isFind = false;
    tosc_message* newMsg = nullptr;
    //=========================================================================
    if ( tosc_bundle* data =_checkUserData_AsTOSC_Bundle(lua_touserdata( L, 1 )) )
    {
        newMsg = new tosc_message();
        isFind = tosc_getNextMessage( data ,newMsg);
        //=====================================================================
        if( isFind )
            G_MSG_Vector.push_back( newMsg );
        else
            delete newMsg;
        //=====================================================================
    }
    else
    {
        lua_pushnil(L);
        luaL_error( L, "TinyOSC Error: #1 argument is not OSC_Bundle Class." );
        return 1;
    }
    //=========================================================================
    lua_pushboolean( L, isFind );
    ( isFind ) ? lua_pushlightuserdata( L, (void*) newMsg ) : lua_pushnil(L);
    //=========================================================================
    return 2; // return  is 2
}
//=============================================================================

//=============================================================================
// void     tosc_parseBundle     ( tosc_bundle *b, char *buffer, const int len);
// void     tosc_writeBundle     ( tosc_bundle *b, uint64_t timetag, char *buffer, const int len);
// uint32_t tosc_writeNextMessage( tosc_bundle *b,const char *address, const char *format, ...);
//=============================================================================

//=============================================================================
static const struct luaL_Reg functionInfo[] = 
{
    //=========================================================================
      { "create_OSCMessage_SocketBuffer"  , Create_OSC_MessageBuffer_1 }
    //=========================================================================
    , { "Message_create"            , TOSC_Message_create             }
    , { "Message_delete"            , TOSC_Message_delete             }
    //=========================================================================
    , { "Message_getAddress"        , TOSC_Message_getAddress         }
    , { "Message_getFormat"         , TOSC_Message_getFormat          }
    , { "Message_getLength"         , TOSC_Message_getLength          }
    , { "Message_getNextInt32"      , TOSC_Message_getNextInt32       }
    , { "Message_getNextInt64"      , TOSC_Message_getNextInt64       }
    , { "Message_getNextTimeTag"    , TOSC_Message_getNextTimeTag     }
    , { "Message_getNextFloat"      , TOSC_Message_getNextFloat       }
    , { "Message_getNextDouble"     , TOSC_Message_getNextDouble      }
    , { "Message_getNextString"     , TOSC_Message_getNextString      }
    , { "Message_getNextMidi"       , TOSC_Message_getNextMidi        }
    , { "Message_getNextBlob"       , TOST_Message_getNextBlob        }
    //=========================================================================
    , { "Message_parseMessage"      , TOST_Message_parseMessage       }
    , { "Message_getPrintMessage"   , TOST_Message_getPrintMessage    }
    //=========================================================================
    , { "Bundle_create"             , TOSC_Bundle_create              }
    , { "Bundle_delete"             , TOSC_Bundle_delete              }
    //=========================================================================
    , { "Bundle_isBundle"           , TOSC_isBundle                   }
    , { "Bundle_getTimeTag"         , TOSC_Bundle_getTimeTag          }
    , { "Bundle_getBundleLengt"     , TOSC_Bundle_getBundleLength     }
    , { "Bundle_getNextMessage"     , TOSC_Bundle_getNextMessage      }
    //=========================================================================
    , {NULL, NULL}
    //=========================================================================
};
//=============================================================================

//=============================================================================
#ifndef BUILD_AS_DEFOLD_EXT
    //=========================================================================
    #ifdef __cplusplus
    extern "C" {
    #endif
    //=========================================================================
    LUALIB_API int luaopen_TinyOSC (lua_State *L) 
    {
        //=====================================================================
        luaL_register( L, "TinyOSC", functionInfo );
        //=====================================================================
        return 1;
    }
    //=========================================================================
    #ifdef __cplusplus
    }
    #endif
    //=========================================================================
#else
    //=========================================================================
    dmExtension::Result AppInitializeTinyOscExtension(dmExtension::AppParams* params)
    {
        return dmExtension::RESULT_OK;
    }
    //=========================================================================
    dmExtension::Result AppFinalizeTinyOscExtension(dmExtension::AppParams* params) 
    {
        clearAllStored_OSG_MessageInstance();
        cleareAllStored_OSC_BundleINstance();
        //=====================================================================
        return dmExtension::RESULT_OK;
    }
    //=========================================================================
    dmExtension::Result InitializeTinyOscExtension   (dmExtension::Params* params) 
    {
        int top = lua_gettop(params->m_L);
        luaL_register(params->m_L, MODULE_NAME_STR, functionInfo);
        lua_pop(params->m_L, 1);
        //=====================================================================
        assert(top == lua_gettop(params->m_L));
        printf("Registered %s Extension\n", MODULE_NAME_STR );
        return dmExtension::RESULT_OK;
    }
    //=========================================================================
    dmExtension::Result UpdateTinyOscExtension     (dmExtension::Params* params) 
    {
        return dmExtension::RESULT_OK;
    }
    //=========================================================================
    dmExtension::Result FinalizeTinyOscExtension   (dmExtension::Params* params) 
    {
        return dmExtension::RESULT_OK;
    }
    //=========================================================================

    //=========================================================================
    // DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
    DM_DECLARE_EXTENSION ( TinyOSC
                         , MODULE_NAME_STR
                         , AppInitializeTinyOscExtension
                         , AppFinalizeTinyOscExtension
                         , InitializeTinyOscExtension
                         , 0 // UpdateTinyOscExtension
                         , 0
                         , FinalizeTinyOscExtension);
    //=========================================================================
#endif
//=============================================================================


