/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "URIHelper.h"

#include <decaf/lang/Integer.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Exception.h>
#include <decaf/internal/net/URIEncoderDecoder.h>
#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/exceptions/NumberFormatException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::net;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
URIHelper::URIHelper() {
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::parseURI( const std::string& uri, bool forceServer )
    throw( URISyntaxException ) {

//    std::string temp = uri;
//
//    std::size_t index, index1, index2, index3;
//    // parse into Fragment, Scheme, and SchemeSpecificPart
//    // then parse SchemeSpecificPart if necessary
//
//    // Fragment
//    index = temp.find( '#' );
//    if( index != std::string::npos ) {
//        // remove the fragment from the end
//
//        // TODO - Replace vars from URI.java with something in this class
////        fragment = temp.substr( index + 1, std::string::npos );
////        validateFragment( uri, fragment, index + 1 );
//        temp = temp.substr( 0, index );
//    }
//
//    // Scheme and SchemeSpecificPart
//    index = index1 = temp.find( ':' );
//    index2 = temp.find( '/' );
//    index3 = temp.find( '?' );
//
//    // if a '/' or '?' occurs before the first ':' the uri has no
//    // specified scheme, and is therefore not absolute
//    if( index != std::string::npos &&
//        ( index2 >= index || index2 == std::string::npos ) &&
//        ( index3 >= index || index3 == std::string::npos ) ) {
//
//        // the characters up to the first ':' comprise the scheme
//        absolute = true;
//        scheme = temp.substr( 0, index );
//        if( scheme.length() == 0 ) {
//            throw URISyntaxException(
//                __FILE__, __LINE__,
//                uri, "Scheme not specified.", index );
//        }
//        validateScheme( uri, scheme, 0 );
//        schemespecificpart = temp.substr( index + 1, std::string::npos );
//        if( schemespecificpart.length() == 0 ) {
//            throw URISyntaxException(
//                __FILE__, __LINE__,
//                uri, "Scheme specific part is invalid..", index + 1 );
//        }
//    } else {
//        absolute = false;
//        schemespecificpart = temp;
//    }
//
//    if( scheme == "" || schemespecificpart.length() > 0 &&
//        schemespecificpart.at( 0 ) == '/' ) {
//
//        opaque = false;
//        // the URI is hierarchical
//
//        // Query
//        temp = schemespecificpart;
//        index = temp.find( '?' );
//        if( index != std::string::npos ) {
//            query = temp.substr( index + 1, std::string::npos );
//            temp = temp.substr( 0, index );
//            validateQuery( uri, query, index2 + 1 + index );
//        }
//
//        // Authority and Path
//        if( temp.at(0) == '/' && temp.at(1) == '/' ) {
//
//            index = temp.find( '/', 2 );
//            if( index != std::string::npos ) {
//                authority = temp.substr( 2, index );
//                path = temp.substr( index, std::string::npos );
//            } else {
//                authority = temp.substr( 2, std::string::npos );
//                if( authority.length() == 0 && query == "" && fragment == "" ) {
//                    throw URISyntaxException(
//                        __FILE__, __LINE__,
//                        uri, "Scheme specific part is invalid..", uri.length() );
//                }
//
//                path = "";
//                // nothing left, so path is empty (not null, path should
//                // never be null)
//            }
//
//            if( authority.length() == 0 ) {
//                authority = "";
//            } else {
//                validateAuthority( uri, authority, index1 + 3 );
//            }
//        } else { // no authority specified
//            path = temp;
//        }
//
//        int pathIndex = 0;
//        if( index2 != std::string::npos ) {
//            pathIndex += index2;
//        }
//
//        if( index != std::string::npos ) {
//            pathIndex += index;
//        }
//
//        validatePath( uri, path, pathIndex );
//
//    } else { // if not hierarchical, URI is opaque
//        opaque = true;
//        validateSsp( uri, schemespecificpart, index2 + 2 + index );
//    }
//
//    parseAuthority( forceServer, authority );
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::validateScheme( const std::string& uri, const std::string& scheme, int index )
    throw( URISyntaxException ) {

    // first char needs to be an alpha char
    if( !Character::isLetter( scheme.at(0) ) ) {
        throw URISyntaxException(
            __FILE__, __LINE__,
            uri, "Schema must start with a Letter.", index );
    }

    try {
        URIEncoderDecoder::validateSimple( scheme, "+-." );
    } catch( URISyntaxException& e ) {
        throw URISyntaxException(
            __FILE__, __LINE__,
            uri, "Invalid Schema", index + e.getIndex() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::validateSsp( const std::string& uri, const std::string& ssp,
                             std::size_t index, const std::string& allLegal )
    throw( URISyntaxException ) {

    try {
        URIEncoderDecoder::validate( ssp, allLegal );
    } catch( URISyntaxException& e ) {
        throw URISyntaxException(
            __FILE__, __LINE__,
            uri, "Invalid URI Ssp", index + e.getIndex() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::validateAuthority( const std::string& uri, const std::string& authority,
                                   std::size_t index, const std::string& allLegal )
    throw( URISyntaxException ) {

    try {
        // "@[]" + someLegal
        URIEncoderDecoder::validate( authority, allLegal );
    } catch( URISyntaxException& e ) {
        throw URISyntaxException(
            __FILE__, __LINE__,
            uri, "Invalid URI Authority", index + e.getIndex() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::validatePath( const std::string& uri, const std::string& path,
                              std::size_t index, const std::string& allLegal )
    throw( URISyntaxException ) {

    try {
        // "/@" + someLegal
        URIEncoderDecoder::validate( path, allLegal );
    } catch( URISyntaxException& e ) {
        throw URISyntaxException(
            __FILE__, __LINE__,
            uri, "Invalid URI Path", index + e.getIndex() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::validateQuery( const std::string& uri, const std::string& query,
                               std::size_t index, const std::string& allLegal )
    throw( URISyntaxException ) {

    try {
        URIEncoderDecoder::validate( query, allLegal );
    } catch( URISyntaxException& e ) {
        throw URISyntaxException(
            __FILE__, __LINE__,
            uri, "Invalid URI Query", index + e.getIndex() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::validateFragment( const std::string& uri, const std::string& fragment,
                                  std::size_t index, const std::string& allLegal )
    throw( URISyntaxException ) {

    try {
        URIEncoderDecoder::validate( fragment, allLegal );
    } catch( URISyntaxException& e ) {
        throw URISyntaxException(
            __FILE__, __LINE__,
            uri, "Invalid URI Fragment", index + e.getIndex() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIHelper::parseAuthority( bool forceServer, const std::string& authority )
    throw( URISyntaxException ) {

    try{

        if( authority == "" ) {
            return;
        }

        std::string temp, tempUserinfo = "", tempHost = "";
        std::size_t index, hostindex = 0;
        int tempPort = -1;

        temp = authority;
        index = temp.find( '@' );
        if( index != std::string::npos ) {
            // remove user info
            tempUserinfo = temp.substr( 0, index );
            validateUserinfo( authority, tempUserinfo, 0 );
            temp = temp.substr( index + 1, std::string::npos ); // host[:port] is left
            hostindex = index + 1;
        }

        index = temp.find_last_of( ':' );
        std::size_t endindex = temp.find( ']' );

        if( index != std::string::npos && endindex < index ) {
            // determine port and host
            tempHost = temp.substr( 0, index );

            if( index < ( temp.length() - 1 ) ) { // port part is not empty
                try {

                    tempPort = Integer::parseInt( temp.substr( index + 1, std::string::npos ) );
                    if( tempPort < 0 ) {

                        if( forceServer ) {
                            throw URISyntaxException(
                                __FILE__, __LINE__,
                                authority, "Port number is missing",
                                hostindex + index + 1 );
                        }

                        return;
                    }
                } catch( NumberFormatException& e ) {

                    if( forceServer ) {
                        throw URISyntaxException(
                            __FILE__, __LINE__,
                            authority, "Port number is malformed.",
                            hostindex + index + 1 );
                    }

                    return;
                }
            }

        } else {
            tempHost = temp;
        }

        if( tempHost == "" ) {
            if( forceServer ) {
                throw URISyntaxException(
                    __FILE__, __LINE__,
                    authority, "Host name is empty", hostindex );
            }
            return;
        }

        if( !isValidHost( forceServer, tempHost ) ) {
            return;
        }

        // this is a server based uri,
        // fill in the userinfo, host and port fields

        // TODO - Get the parsed Data back to the caller.
//        userinfo = tempUserinfo;
//        host = tempHost;
//        port = tempPort;
//        serverAuthority = true;
    }
    DECAF_CATCH_RETHROW( URISyntaxException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, URISyntaxException )
    DECAF_CATCHALL_THROW( URISyntaxException )
}

////////////////////////////////////////////////////////////////////////////////
void validateUserinfo( const std::string& uri, const std::string& userinfo, std::size_t index )
    throw( URISyntaxException ) {

    for( std::size_t i = 0; i < userinfo.length(); i++ ) {

        char ch = userinfo.at( i );
        if( ch == ']' || ch == '[' ) {
            throw URISyntaxException(
                __FILE__, __LINE__,
                uri, "User Info cannot contain '[' or ']'", index + i );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidHost( bool forceServer, const std::string& host )
    throw( URISyntaxException ) {

    try{

        if( host.at( 0 ) == '[' ) {

            // ipv6 address
            if( host.at( host.length() - 1 ) != ']' ) {
                throw URISyntaxException(
                    __FILE__, __LINE__,
                    host, "Host address does not end in ']'", 0 );
            }

            if( !isValidIP6Address( host ) ) {
                throw URISyntaxException(
                    __FILE__, __LINE__,
                    host, "Host IPv6 address is not valid" );
            }

            return true;
        }

        // '[' and ']' can only be the first char and last char
        // of the host name
        if( host.find( '[' ) != std::string::npos ||
            host.find( ']' ) != std::string::npos ) {

            throw URISyntaxException(
                __FILE__, __LINE__,
                host, "Unexpected '[' or ']' found in address" );
        }

        std::size_t index = host.find_last_of( '.' );

        if( index == std::string::npos || index == host.length() - 1 ||
            !Character::isDigit( host.at( index + 1 ) ) ) {

            // domain name
            if( isValidDomainName( host ) ) {
                return true;
            }

            if( forceServer ) {
                throw URISyntaxException(
                    __FILE__, __LINE__,
                    host, "Host address is not valid" );
            }

            return false;
        }

        // IPv4 address
        if( isValidIPv4Address( host ) ) {
            return true;
        }

        if( forceServer ) {
            throw URISyntaxException(
                __FILE__, __LINE__,
                host, "Host IPv4 address is not valid" );
        }

        return false;
    }
    DECAF_CATCH_RETHROW( URISyntaxException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, URISyntaxException )
    DECAF_CATCHALL_THROW( URISyntaxException )
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidDomainName( const std::string& host ) {

    try {
        URIEncoderDecoder::validateSimple( host, "-." );
    } catch( URISyntaxException& e ) {
        return false;
    }

    std::string label = "";
    StringTokenizer st( host, "." );

    while( st.hasMoreTokens() ) {
        label = st.nextToken();
        if( *(label.begin()) == '-' || *(label.rbegin()) == '-' ) {
            return false;
        }
    }

    if( label != host ) {
        if( Character::isDigit( label.at( 0 ) ) ) {
            return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool isValidIPv4Address( const std::string& host ) {

    int index;
    int index2;

    try {

        int num;
        index = host.find( '.' );
        num = Integer::parseInt( host.substr( 0, index ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

        index2 = host.find( '.', index + 1 );
        num = Integer::parseInt( host.substr( index + 1, index2 ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

        index = host.find( '.', index2 + 1 );
        num = Integer::parseInt( host.substr( index2 + 1, index ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

        num = Integer::parseInt( host.substr( index + 1, std::string::npos ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

    } catch( Exception& e ) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidIP6Address( const std::string& ipAddress ) {

    int length = ipAddress.length();
    bool doubleColon = false;
    int numberOfColons = 0;
    int numberOfPeriods = 0;
    std::string word = "";
    char c = 0;
    char prevChar = 0;
    int offset = 0; // offset for [] ip addresses

    if( length < 2 ) {
        return false;
    }

    for( int i = 0; i < length; i++ ) {

        prevChar = c;
        c = ipAddress.at( i );

        switch( c ) {

            // case for an open bracket [x:x:x:...x]
            case '[':
                if( i != 0 ) {
                    return false; // must be first character
                }
                if( ipAddress.at( length - 1 ) != ']' ) {
                    return false; // must have a close ]
                }
                if( ( ipAddress.at( 1 ) == ':' ) && ( ipAddress.at( 2 )
                    != ':' ) ) {
                    return false;
                }
                offset = 1;
                if( length < 4 ) {
                    return false;
                }
                break;

                // case for a closed bracket at end of IP [x:x:x:...x]
            case ']':
                if( i != length - 1 ) {
                    return false; // must be last character
                }
                if( ipAddress.at( 0 ) != '[' ) {
                    return false; // must have a open [
                }
                break;

                // case for the last 32-bits represented as IPv4
                // x:x:x:x:x:x:d.d.d.d
            case '.':
                numberOfPeriods++;
                if( numberOfPeriods > 3 ) {
                    return false;
                }
                if( !isValidIP4Word( word ) ) {
                    return false;
                }
                if( numberOfColons != 6 && !doubleColon ) {
                    return false;
                }
                // a special case ::1:2:3:4:5:d.d.d.d allows 7 colons
                // with
                // an IPv4 ending, otherwise 7 :'s is bad
                if( numberOfColons == 7 &&
                    ipAddress.at( 0 + offset ) != ':' &&
                    ipAddress.at( 1 + offset ) != ':' ) {

                    return false;
                }
                word = "";
                break;

            case ':':
                numberOfColons++;
                if( numberOfColons > 7 ) {
                    return false;
                }
                if( numberOfPeriods > 0 ) {
                    return false;
                }
                if( prevChar == ':' ) {
                    if( doubleColon ) {
                        return false;
                    }
                    doubleColon = true;
                }
                word = "";
                break;

            default:
                if( word.length() > 3 ) {
                    return false;
                }
                if( !isValidHexChar( c ) ) {
                    return false;
                }
                word += c;
        }
    }

    // Check if we have an IPv4 ending
    if( numberOfPeriods > 0 ) {
        if( numberOfPeriods != 3 || !isValidIP4Word( word ) ) {
            return false;
        }
    } else {
        // If we're at then end and we haven't had 7 colons then there
        // is a problem unless we encountered a doubleColon
        if( numberOfColons != 7 && !doubleColon ) {
            return false;
        }

        // If we have an empty word at the end, it means we ended in
        // either a : or a .
        // If we did not end in :: then this is invalid
        if( word == "" && ipAddress.at( length - 1 - offset ) != ':' &&
            ipAddress.at( length - 2 - offset ) != ':' ) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidIP4Word( const std::string& word ) {

    if( word.length() < 1 || word.length() > 3 ) {
        return false;
    }

    for( std::size_t i = 0; i < word.length(); i++ ) {

        if( !Character::isDigit( word.at( i ) ) ) {
            return false;
        }
    }

    if( Integer::parseInt( word ) > 255 ) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidHexChar( char c ) {

    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f');
}