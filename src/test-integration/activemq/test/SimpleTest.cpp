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

#include "SimpleTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/UUID.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testAutoAck() {

    try {

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );

        CMSListener listener( session );

        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        consumer->setMessageListener( &listener );
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );
        auto_ptr<cms::BytesMessage> bytesMessage( session->createBytesMessage() );

        for( unsigned int i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
            producer->send( txtMessage.get() );
        }

        for( unsigned int i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
            producer->send( bytesMessage.get() );
        }

        // Wait for the messages to get here
        listener.asyncWaitForMessages( IntegrationCommon::defaultMsgCount * 2 );

        unsigned int numReceived = listener.getNumReceived();
        CPPUNIT_ASSERT( numReceived == IntegrationCommon::defaultMsgCount * 2 );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testClientAck() {

    try {

        cmsProvider->setAckMode( cms::Session::CLIENT_ACKNOWLEDGE );
        cmsProvider->reconnectSession();

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );

        CMSListener listener( session );

        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        consumer->setMessageListener( &listener );
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );
        auto_ptr<cms::BytesMessage> bytesMessage( session->createBytesMessage() );

        for( unsigned int i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
            producer->send( txtMessage.get() );
        }

        for( unsigned int i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
            producer->send( bytesMessage.get() );
        }

        // Wait for the messages to get here
        listener.asyncWaitForMessages( IntegrationCommon::defaultMsgCount * 2 );

        unsigned int numReceived = listener.getNumReceived();
        CPPUNIT_ASSERT( numReceived == IntegrationCommon::defaultMsgCount * 2 );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testProducerWithNullDestination() {

    try{

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );

        CMSListener listener( session );

        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        consumer->setMessageListener( &listener );
        cms::MessageProducer* producer = cmsProvider->getNoDestProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );

        producer->send( cmsProvider->getDestination(), txtMessage.get() );

        // Wait for the messages to get here
        listener.asyncWaitForMessages( 1 );

        unsigned int numReceived = listener.getNumReceived();
        CPPUNIT_ASSERT( numReceived == 1 );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testSyncReceive() {

    try {

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );

        // Send some text messages
        producer->send( txtMessage.get() );

        auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testSyncReceiveClientAck() {

    try {

        cmsProvider->setAckMode( cms::Session::CLIENT_ACKNOWLEDGE );
        cmsProvider->reconnectSession();

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );
        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage( "TEST MESSAGE" ) );

        // Send some text messages
        producer->send( txtMessage.get() );

        auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testMultipleConnections() {

    try {

        // Create CMS Object for Comms
        cms::ConnectionFactory* factory = cmsProvider->getConnectionFactory();
        auto_ptr<cms::Connection> connection1( factory->createConnection() );
        connection1->start();

        auto_ptr<cms::Connection> connection2( factory->createConnection() );
        connection2->start();

        CPPUNIT_ASSERT( connection1->getClientID() != connection2->getClientID() );

        auto_ptr<cms::Session> session1( connection1->createSession() );
        auto_ptr<cms::Session> session2( connection1->createSession() );

        auto_ptr<cms::Topic> topic( session1->createTopic( UUID::randomUUID().toString() ) );

        auto_ptr<cms::MessageConsumer> consumer1( session1->createConsumer( topic.get() ) );
        auto_ptr<cms::MessageConsumer> consumer2( session2->createConsumer( topic.get() ) );

        auto_ptr<cms::MessageProducer> producer( session2->createProducer( topic.get() ) );
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> textMessage( session2->createTextMessage() );

        // Send some text messages
        producer->send( textMessage.get() );

        auto_ptr<cms::Message> message( consumer1->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );

        message.reset( consumer2->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testMultipleSessions() {
    try {

        // Create CMS Object for Comms
        auto_ptr<cms::Session> session1( cmsProvider->getConnection()->createSession() );
        auto_ptr<cms::Session> session2( cmsProvider->getConnection()->createSession() );

        auto_ptr<cms::Topic> topic( session1->createTopic( UUID::randomUUID().toString() ) );

        auto_ptr<cms::MessageConsumer> consumer1( session1->createConsumer( topic.get() ) );
        auto_ptr<cms::MessageConsumer> consumer2( session2->createConsumer( topic.get() ) );

        auto_ptr<cms::MessageProducer> producer( session2->createProducer( topic.get() ) );
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> textMessage( session2->createTextMessage() );

        // Send some text messages
        producer->send( textMessage.get() );

        auto_ptr<cms::Message> message( consumer1->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );

        message.reset( consumer2->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testReceiveAlreadyInQueue() {

    try {

        // Create CMS Object for Comms
        cms::ConnectionFactory* factory = cmsProvider->getConnectionFactory();
        auto_ptr<cms::Connection> connection( factory->createConnection() );

        auto_ptr<cms::Session> session( connection->createSession() );
        auto_ptr<cms::Topic> topic( session->createTopic( UUID::randomUUID().toString() ) );
        auto_ptr<cms::MessageConsumer> consumer( session->createConsumer( topic.get() ) );
        auto_ptr<cms::MessageProducer> producer( session->createProducer( topic.get() ) );
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );
        auto_ptr<cms::TextMessage> textMessage( session->createTextMessage() );

        // Send some text messages
        producer->send( textMessage.get() );

        Thread::sleep( 250 );

        connection->start();

        auto_ptr<cms::Message> message( consumer->receive( 2000 ) );
        CPPUNIT_ASSERT( message.get() != NULL );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void SimpleTest::testQuickCreateAndDestroy() {

    try{

        cms::ConnectionFactory* factory = cmsProvider->getConnectionFactory();
        auto_ptr<cms::Connection> connection( factory->createConnection() );
        auto_ptr<cms::Session> session( connection->createSession() );

        session.reset( NULL );
        connection.reset( NULL );

        connection.reset( factory->createConnection() );
        session.reset( connection->createSession() );
        connection->start();

        session.reset( NULL );
        connection.reset( NULL );

        for( int i = 0; i < 50; ++i ) {
            CMSProvider lcmsProvider( this->getBrokerURL() );
            lcmsProvider.getSession();
            lcmsProvider.getConsumer();
            lcmsProvider.getProducer();
        }

    } catch ( CMSException& e ) {
        e.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}