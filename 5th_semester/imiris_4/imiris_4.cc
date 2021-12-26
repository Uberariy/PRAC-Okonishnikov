#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"

#include <iostream>
#include <string>
#include <random>
#include <cstdio>
#include <cstdlib>

#define CHAN_LENGTH 30 // "L"
#define CHAN_SPEED 10  // "B"
#define MODEL_TIME 10
#define PACK_SIZE 1500  // "l"
#define CSMA_NUM 25
#define ALPHA 100

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("AppCSMA");

long total_packet_sent = 0;
long total_packet_lost = 0;
long total_dublicates = 0;
long host_packet_sent[CSMA_NUM];
long host_packet_lost[CSMA_NUM];
long host_dublicates[CSMA_NUM];
long host_max_queue[CSMA_NUM];
long host_queue[CSMA_NUM];
long max_buffer = 0;

class AppCSMA : public Application {
    virtual void StartApplication();
    virtual void StopApplication();

    Ptr<Socket> socket;
    Address addr;
    EventId send_event;
    bool run;
    int packets_sent;
    std::default_random_engine *gen;
    std::exponential_distribution<double> *distr;
    Ptr<Queue<Packet>> buff;
    std::string name;
    int host_id;
public:
    AppCSMA(): socket(0), addr(), send_event(), run(false), packets_sent(0)
    {}

    void Init(Ptr<Socket> socket_, Address addr_, std::default_random_engine *gen_,
              std::exponential_distribution<double> *distr_, Ptr<Queue<Packet>> buff_, std::string name_, int host_id_)
    {
        socket = socket_;       addr = addr_;        gen = gen_;                distr = distr_;
        buff = buff_;           name = name_;        host_id = host_id_;
    }
    
    void SendPacket()
    {
        Ptr<Packet> packet = Create<Packet>(PACK_SIZE);
        socket->Send(packet);
        total_packet_sent++;
        if(run) {
            Time Next(MilliSeconds(1000 * (*distr)(*gen)));
            host_queue[host_id] += buff->GetNPackets();
            host_packet_sent[host_id]++;
            if (buff->GetNPackets() > host_max_queue[host_id])
                host_max_queue[host_id] = buff->GetNPackets();
            if (buff->GetNPackets() > max_buffer)
                max_buffer = buff->GetNPackets();
            send_event = Simulator::Schedule(Next, &AppCSMA::SendPacket, this);
        }
    }
    
    virtual ~AppCSMA()
    {   
        socket = 0; 
        delete gen; 
        delete distr;   
    }
};

void AppCSMA::StartApplication()
{
    run = true;
    packets_sent = 0;
    socket->Bind();
    socket->Connect(addr);
    socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket>>());
    socket->SetAllowBroadcast(true);
    send_event = Simulator::Schedule(Seconds(0.0), &AppCSMA::SendPacket, this);
}

void AppCSMA::StopApplication()
{
    run = false;
    if(send_event.IsRunning())
        Simulator::Cancel(send_event);
    if(socket)
        socket->Close();
}

static void OnDuplicate(std::string context, Ptr<const Packet> p)
{ 
    total_dublicates++; 
    std::string::size_type pos = context.find(':');
    int i = std::stoi(context.substr(5, pos-1));
    host_dublicates[i]++;
}

static void OnDrop(std::string context, Ptr<const Packet> p)
{ 
    total_packet_lost++; 
    std::string::size_type pos = context.find(':');
    int i = std::stoi(context.substr(5, pos-1));
    host_packet_lost[i]++;
}

int main(int argc, char **argv)
{
    int i; // Iterator
    for (i = 0; i < CSMA_NUM-1; i++) // Prefill:
    {
        host_packet_sent[i] = host_max_queue[i] = host_queue[i] = host_packet_lost[i] = host_dublicates[i] = 0;
    }

    std::cout << "Number of Hosts:\t" << CSMA_NUM << "\n";
    std::cout << "Exponential par. ALPHA:\t" << ALPHA << "\n";
    std::cout << "Channel Lenght (L):\t" << CHAN_LENGTH << "\n";
    std::cout << "Channel Speed (B):\t" << CHAN_SPEED << "\n";
    std::cout << "Packet Size (l):\t" << PACK_SIZE << "\n";
    std::cout << "Model Time:\t" << MODEL_TIME << "\n";
    std::cout << "[===========] STARTING SIMULATION [===========]\n";

    LogComponentEnable("AppCSMA", LOG_LEVEL_INFO);

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(300))); // Задержка распространения
    csma.SetQueue("ns3::DropTailQueue");

    NodeContainer nodeCont;
    nodeCont.Create(CSMA_NUM);
    NetDeviceContainer devCont = csma.Install(nodeCont);

    std::vector<Ptr<Queue<Packet>>> queue;
    Ptr<RateErrorModel> errors = CreateObject<RateErrorModel>();
    errors->SetAttribute("ErrorRate", DoubleValue(0.0000000001)); // При коллизии повторная передача произойдет с заданной вероятностью
    for (i = 0; i < CSMA_NUM; i++) 
    {
        devCont.Get(i)->SetAttribute("ReceiveErrorModel", PointerValue(errors));
        Ptr<DropTailQueue<Packet>> buff = CreateObject<DropTailQueue<Packet>>();
        buff->SetMaxSize(QueueSize("100p")); // Предположение
        buff->TraceConnect("Drop", "Host " + std::to_string(i) + ": ", MakeCallback(&OnDrop));
        queue.push_back(buff);
        devCont.Get(i)->SetAttribute("TxQueue", PointerValue(buff));
    }

    InternetStackHelper IntStack;
    IntStack.Install(nodeCont);

    Ipv4AddressHelper NetAddr;
    NetAddr.SetBase("10.10.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = NetAddr.Assign(devCont);

    uint16_t HostPort = 8000;
    Address HostAddress(InetSocketAddress(interfaces.GetAddress(CSMA_NUM - 1), HostPort));
    PacketSinkHelper packetSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), HostPort));
    ApplicationContainer HostApp = packetSinkHelper.Install(nodeCont.Get(CSMA_NUM - 1));
    HostApp.Start(Seconds(0.0));
    HostApp.Stop(Seconds(MODEL_TIME));

    for (i = 0; i < CSMA_NUM-1; i++) 
    {
        Ptr<Socket> ns3UdpSocket = Socket::CreateSocket(nodeCont.Get(i), UdpSocketFactory::GetTypeId());
        Ptr<AppCSMA> appCSMA = CreateObject<AppCSMA>();
        appCSMA->Init(ns3UdpSocket, HostAddress, new std::default_random_engine(i), 
                  new std::exponential_distribution<double>(ALPHA), queue[i], "Host " + std::to_string(i), i);
        nodeCont.Get(i)->AddApplication(appCSMA);
        appCSMA->SetStartTime(Seconds(0.0));
        appCSMA->SetStopTime(Seconds(MODEL_TIME));
        devCont.Get(i)->TraceConnect("MacTxBackoff", "Host " + std::to_string(i) + ": ", MakeCallback(&OnDuplicate));
    }

    AsciiTraceHelper ascii;
    csma.EnableAsciiAll(ascii.CreateFileStream("fifth.tr"));
    Simulator::Stop(Seconds(MODEL_TIME));
    Simulator::Run();
    Simulator::Destroy();

    double M_buffer = 0;
    double Max_M_buffer = 0;
    double total_fraction = 0;
    for (i = 0; i < CSMA_NUM-1; i++)
    {
        total_fraction += (double(host_packet_lost[i]) + double(host_dublicates[i])) / ((double(host_packet_sent[i]) + double(host_packet_lost[i]) + double(host_dublicates[i])) * CSMA_NUM);
        Max_M_buffer += double(host_max_queue[i]) / CSMA_NUM;
        M_buffer += double(host_queue[i]) / double(host_packet_sent[i]);
    }
    M_buffer = M_buffer / CSMA_NUM;

    std::cout << "[=============] SIMULATION ENDS [=============]\n";
    std::cout << "Packets Sent (No dups):\t" << total_packet_sent << " \n";
    std::cout << "   - Packets Dropped:\t" << total_packet_lost << " \n";
    std::cout << "   - Duplicates:\t" << total_dublicates << " \n";
    std::cout << "[CHAR1] Fraction of Lost packets (1):\t" << (double) (total_packet_lost + total_dublicates) / (total_packet_lost + total_packet_sent + total_dublicates) << " \n";
    std::cout << "[CHAR1] Fraction of Lost packets (2):\t" << (double) total_fraction << " \n";
    std::cout << "[CHAR3] Averege Buffer Queue:\t" << M_buffer << " \n";
    std::cout << "[CHAR3] Max Average Buffer Queue:\t" << Max_M_buffer << " \n";
    std::cout << "[CHAR3] Max Buffer Queue:\t" << max_buffer << " \n";
    return 0;
}