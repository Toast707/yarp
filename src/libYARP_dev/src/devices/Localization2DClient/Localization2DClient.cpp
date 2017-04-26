/*
 * Copyright (C) 2017 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Marco Randazzo <marco.randazzo@iit.it>
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "Localization2DClient.h"
#include <yarp/dev/ILocalization2D.h>
#include <yarp/os/Log.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/LockGuard.h>

/*! \file Navigation2DClient.cpp */

using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;


//------------------------------------------------------------------------------------------------------------------------------

bool yarp::dev::Localization2DClient::open(yarp::os::Searchable &config)
{
    m_local_name.clear();
    m_localization_server_name.clear();

    m_local_name           = config.find("local").asString().c_str();
    m_localization_server_name = config.find("localization_server").asString().c_str();

    if (m_local_name == "")
    {
        yError("Navigation2DClient::open() error you have to provide a valid 'local' param");
        return false;
    }

    if (m_localization_server_name == "")
    {
        yError("Navigation2DClient::open() error you have to provide valid 'localization_server' param");
        return false;
    }

    if (config.check("period"))
    {
        m_period = config.find("period").asInt();
    }
    else
    {
        m_period = 10;
        yWarning("Navigation2DClient: using default period of %d ms" , m_period);
    }

    ConstString
            local_rpc_3,
            remote_rpc_3,
            remote_streaming_name,
            local_streaming_name;

    local_rpc_3           = m_local_name           + "/localization/rpc";
    remote_rpc_3          = m_localization_server_name + "/rpc";
    remote_streaming_name = m_localization_server_name + "/stream:o";
    local_streaming_name  = m_local_name           + "/stream:i";

    if (!m_rpc_port_localization_server.open(local_rpc_3.c_str()))
    {
        yError("Navigation2DClient::open() error could not open rpc port %s, check network", local_rpc_3.c_str());
        return false;
    }

    /*
    //currently unused
    bool ok=Network::connect(remote_streaming_name.c_str(), local_streaming_name.c_str(), "tcp");
    if (!ok)
    {
        yError("Navigation2DClient::open() error could not connect to %s", remote_streaming_name.c_str());
        return false;
    }*/

    bool ok = true;

    ok = Network::connect(local_rpc_3.c_str(), remote_rpc_3.c_str());
    if (!ok)
    {
        yError("Navigation2DClient::open() error could not connect to %s", remote_rpc_3.c_str());
        return false;
    }

    return true;
}

bool  yarp::dev::Localization2DClient::setInitialPose(Map2DLocation& loc)
{
    yarp::os::Bottle b;
    yarp::os::Bottle resp;

    b.addVocab(VOCAB_INAVIGATION);
    b.addVocab(VOCAB_NAV_SET_INITIAL_POS);
    b.addString(loc.map_id);
    b.addDouble(loc.x);
    b.addDouble(loc.y);
    b.addDouble(loc.theta);

    bool ret = m_rpc_port_localization_server.write(b, resp);
    if (ret)
    {
        if (resp.get(0).asVocab() != VOCAB_OK)
        {
            yError() << "Navigation2DClient::setInitialPose() received error from localization server";
            return false;
        }
    }
    else
    {
        yError() << "Navigation2DClient::setInitialPose() error on writing on rpc port";
        return false;
    }
    return true;
}

bool  yarp::dev::Localization2DClient::getCurrentPosition(Map2DLocation& loc)
{
    yarp::os::Bottle b;
    yarp::os::Bottle resp;

    b.addVocab(VOCAB_INAVIGATION);
    b.addVocab(VOCAB_NAV_GET_CURRENT_POS);

    bool ret = m_rpc_port_localization_server.write(b, resp);
    if (ret)
    {
        if (resp.get(0).asVocab() != VOCAB_OK || resp.size() != 5)
        {
            yError() << "Navigation2DClient::getCurrentPosition() recived error from localization server";
            return false;
        }
        else
        {
            loc.map_id = resp.get(1).asString();
            loc.x = resp.get(2).asDouble();
            loc.y = resp.get(3).asDouble();
            loc.theta = resp.get(4).asDouble();
            return true;
        }
    }
    else
    {
        yError() << "Navigation2DClient::getCurrentPosition() error on writing on rpc port";
        return false;
    }
    return true;
}

bool yarp::dev::Localization2DClient::close()
{
    m_rpc_port_localization_server.close();
    return true;
}

yarp::dev::DriverCreator *createLocalization2DClient()
{
    return new DriverCreatorOf<Localization2DClient>
               (
                   "localization2DClient",
                   "",
                   "localization2DClient"
               );
}
