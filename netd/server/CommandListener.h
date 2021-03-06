/*
* Copyright (C) 2014 MediaTek Inc.
* Modification based on code covered by the mentioned copyright
* and/or permission notice(s).
*/
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _COMMANDLISTENER_H__
#define _COMMANDLISTENER_H__

#include <sysutils/FrameworkListener.h>

#include "NetdCommand.h"
#include "NetworkController.h"
#include "TetherController.h"
#include "NatController.h"
#include "PppController.h"
#include "SoftapController.h"
#include "BandwidthController.h"
#include "IdletimerController.h"
#include "InterfaceController.h"
#include "ResolverController.h"
#include "FirewallController.h"
#include "ClatdController.h"
#include "StrictController.h"
#include "PPPOEController.h"
#include "NetInfoListener.h"
#include "ThroughputMonitor.h"
#include "ThrottleController.h"
#include "IPv6TetherController.h"
/* M: HetComm Feature */
#include "DualOnController.h"

#include "PerfController.h"


class CommandListener : public FrameworkListener {
    static TetherController *sTetherCtrl;
    static NatController *sNatCtrl;
    static PppController *sPppCtrl;
    static SoftapController *sSoftapCtrl;
    static BandwidthController *sBandwidthCtrl;
    static IdletimerController *sIdletimerCtrl;
    static InterfaceController *sInterfaceCtrl;
    static ResolverController *sResolverCtrl;
    static FirewallController *sFirewallCtrl;
    static ClatdController *sClatdCtrl;
    static StrictController *sStrictCtrl;
    static PPPOEController *sPPPOECtrl;
    static NetInfoListener *sNetInfo;
    static IPv6TetherController *sIPv6TetherCtrl;
    static ThroughputMonitor *sThroughputMonitor;
    static ThrottleController *sThrottleCtrl;
    /* M: HetComm Feature */
    static DualOnController *sDualOnCtrl;
    /*perfService feature*/
    static PerfController *sPerfCtrl;


public:
    static NetworkController *sNetCtrl;

    CommandListener();
    virtual ~CommandListener() {}

private:

    class SoftapCmd : public NetdCommand {
    public:
        SoftapCmd();
        virtual ~SoftapCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class InterfaceCmd : public NetdCommand {
    public:
        InterfaceCmd();
        virtual ~InterfaceCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class IpFwdCmd : public NetdCommand {
    public:
        IpFwdCmd();
        virtual ~IpFwdCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class TetherCmd : public NetdCommand {
    public:
        TetherCmd();
        virtual ~TetherCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class NatCmd : public NetdCommand {
    public:
        NatCmd();
        virtual ~NatCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class ListTtysCmd : public NetdCommand {
    public:
        ListTtysCmd();
        virtual ~ListTtysCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class PppdCmd : public NetdCommand {
    public:
        PppdCmd();
        virtual ~PppdCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class BandwidthControlCmd : public NetdCommand {
    public:
        BandwidthControlCmd();
        virtual ~BandwidthControlCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    protected:
        void sendGenericOkFail(SocketClient *cli, int cond);
        void sendGenericOpFailed(SocketClient *cli, const char *errMsg);
        void sendGenericSyntaxError(SocketClient *cli, const char *usageMsg);
    };

    class IdletimerControlCmd : public NetdCommand {
    public:
        IdletimerControlCmd();
        virtual ~IdletimerControlCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class ResolverCmd : public NetdCommand {
    public:
        ResolverCmd();
        virtual ~ResolverCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class FirewallCmd: public NetdCommand {
    public:
        FirewallCmd();
        virtual ~FirewallCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    protected:
        int sendGenericOkFail(SocketClient *cli, int cond);
        static FirewallRule parseRule(const char* arg);
        static FirewallType parseFirewallType(const char* arg);
        static ChildChain parseChildChain(const char* arg);
        // mtk03594: Support enhanced firewall @{
        static FirewallChinaRule parseChain(const char* arg);

    };

    class ClatdCmd : public NetdCommand {
    public:
        ClatdCmd();
        virtual ~ClatdCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class StrictCmd : public NetdCommand {
    public:
        StrictCmd();
        virtual ~StrictCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    protected:
        int sendGenericOkFail(SocketClient *cli, int cond);
        static StrictPenalty parsePenalty(const char* arg);
    };

    class NetworkCommand : public NetdCommand {
    public:
        NetworkCommand();
        virtual ~NetworkCommand() {}
        int runCommand(SocketClient* client, int argc, char** argv);
    private:
        int syntaxError(SocketClient* cli, const char* message);
        int operationError(SocketClient* cli, const char* message, int ret);
        int success(SocketClient* cli);
    };
    class Ipv6FwdCmd : public NetdCommand {
	public:
	    Ipv6FwdCmd();
	    virtual ~Ipv6FwdCmd() {}
	    int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class IPv6TetherCmd : public NetdCommand {
	public:
	    IPv6TetherCmd();
	    virtual ~IPv6TetherCmd() {}
	    int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class NetInfoCmd : public NetdCommand {
    public:
        NetInfoCmd();
        virtual ~NetInfoCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class ThroughputCmd : public NetdCommand {
    public:
        ThroughputCmd();
        virtual ~ThroughputCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    class PPPOEctlCmd : public NetdCommand {
    public:
        PPPOEctlCmd();
        virtual ~PPPOEctlCmd() {}
        int runCommand(SocketClient *c, int argc, char ** argv);
    };

    /* M: HetComm Feature */
    class DualOnCmd : public NetdCommand {
    public:
        DualOnCmd();
        virtual ~DualOnCmd() {}
        int runCommand(SocketClient *cli, int argc, char ** argv);
    };

public:
    static ThrottleController * getThrottleCtrl() {return sThrottleCtrl; }
};

#endif
