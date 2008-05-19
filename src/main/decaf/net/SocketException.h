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
#ifndef _DECAF_NET_SOCKETEXCEPTION_H_
#define _DECAF_NET_SOCKETEXCEPTION_H_

#include <decaf/io/IOException.h>

namespace decaf{
namespace net{

    /**
     * Exception for errors when manipulating sockets.
     */
    class DECAF_API SocketException : public io::IOException
    {
    public:

        SocketException() throw() {}

        SocketException( const lang::Exception& ex ) throw()
        : io::IOException() {
            *(lang::Exception*)this = ex;
        }

        SocketException( const SocketException& ex ) throw()
        : io::IOException() {
            *(lang::Exception*)this = ex;
        }

        SocketException( const char* file, const int lineNumber,
                         const char* msg, ...) throw()
        : io::IOException() {
            va_list vargs;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         */
        virtual SocketException* clone() const {
            return new SocketException( *this );
        }

        virtual ~SocketException() throw() {}

    };

}}


#endif // _DECAF_NET_SOCKETEXCEPTION_H_
