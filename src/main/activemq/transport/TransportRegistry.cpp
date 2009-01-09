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

#include "TransportRegistry.h"

using namespace std;
using namespace activemq;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
TransportRegistry::TransportRegistry() {
}

////////////////////////////////////////////////////////////////////////////////
TransportRegistry::~TransportRegistry() {

    std::vector<TransportFactory*> factories = this->registry.getValues();

    std::vector<TransportFactory*>::iterator iter = factories.begin();

    for( ; iter != factories.end(); ++iter ) {
        delete *iter;
    }

    this->registry.clear();
 }

////////////////////////////////////////////////////////////////////////////////
TransportFactory* TransportRegistry::findFactory( const std::string& name ) const
    throw( decaf::lang::exceptions::NoSuchElementException ) {

    if( !this->registry.containsKey( name ) ) {
        throw NoSuchElementException( __FILE__, __LINE__,
            "No Matching Factory Registered for format := %s", name.c_str() );
    }

    return this->registry.getValue( name );
}

////////////////////////////////////////////////////////////////////////////////
void TransportRegistry::registerFactory( const std::string& name, TransportFactory* factory )
    throw( decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::NullPointerException ) {

    if( name == "" ) {
        throw IllegalArgumentException( __FILE__, __LINE__,
            "TransportFactory name cannot be the empty string" );
    }

    if( factory == NULL ) {
        throw NullPointerException( __FILE__, __LINE__,
            "Supplied TransportFactory pointer was NULL" );
    }

    this->registry.setValue( name, factory );
}

////////////////////////////////////////////////////////////////////////////////
void TransportRegistry::unregisterFactory( const std::string& name ) {
    if( this->registry.containsKey( name ) ) {
        delete this->registry.getValue( name );
        this->registry.remove( name );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> TransportRegistry::getTransportNames() const {
    return this->registry.getKeys();
}

////////////////////////////////////////////////////////////////////////////////
TransportRegistry& TransportRegistry::getInstance() {
    static TransportRegistry registry;
    return registry;
}