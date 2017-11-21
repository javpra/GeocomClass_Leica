#include "geocom_class_leica.h"


GeoCom_Class_Leica::GeoCom_Class_Leica()
{
}

/*********************************************************************************** Split ***********************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::split(char *buf)
{
    std::string reply(buf);
    std::vector<std::string> words,parameters;
    int pos=0,length=0;

    for(int i=0;i<2;i++){ // I know there is two "," before the ":", so I make a while loop with that dimension
        length=reply.find(",",pos);
        words.push_back(reply.substr(pos,length-pos));  // The first components are stored for future applications
        pos=length+1;
    }
    length=reply.find(":",pos);
    words.push_back(reply.substr(pos,length-pos));

    pos=length+1;
    while(length != -1){ //If it doesn't find anything it returns -1 and leaves the while loop. In the last parameter, pos value is 0 (pos = -1 + 1)
        length=reply.find(",",pos);
        parameters.push_back(reply.substr(pos,length-pos));
        pos=length+1;
    }
    return parameters;
}
/******************************************************************************* write and read ******************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::writeAndRead(std::string cmd)
{
    if(clientSocket.writeData(QByteArray(cmd.c_str(), cmd.size()))==false){
        std::cout << "Error sending data through the socket" << std::endl;
    }
    //std::cout << "Printing the data sended to the Total Station: " << buffer << std::endl;
    if(clientSocket.receiveData(serverReply)==false){
        std::cout << "Error reading data from the socket" << std::endl;
    }
    //std::cout << "Printing the data received by the Total Station: " << serverReply << std::endl;
    std::vector<std::string> params= GeoCom_Class_Leica::split(serverReply);
    return params;
}
/******************************************************************************* TMC_GetAngle ********************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::TMC_GetAngle(int mode)
{
//  TMC_GetAngle(1);
    auto req = GeoCom_Class_Leica::createRequest(2107,std::to_string(mode));
    std::vector<std::string> params = GeoCom_Class_Leica::writeAndRead(req);
    return params;
}
/****************************************************************************** TMC_GetStation *******************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::TMC_GetStation() //Units in meters
{
//  TMC_GetStation(1);
    auto req = GeoCom_Class_Leica::createRequest(2009,"");
    std::vector<std::string> params = GeoCom_Class_Leica::writeAndRead(req);
    return params;
}
/****************************************************************************** TMC_SetStation *******************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::TMC_SetStation(double eastCoord, double northCoord, double heightCoord, double instrumentHeight, double offset) //Units in meters
{
//  TMC_GetAngle(1);
    if(offset!=-1)
    {
        instrumentHeight += offset; // offset de altura interno. Tomar una medida del prisma en el suelo y restarle a la Z la altura de la TS medida con una cinta métrica. Esa medida será el offset
    }
    auto req = GeoCom_Class_Leica::createRequest(2010,std::to_string(eastCoord)+","+std::to_string(northCoord)+","+std::to_string(heightCoord)+","+std::to_string(instrumentHeight));
    std::vector<std::string> params = GeoCom_Class_Leica::writeAndRead(req);
    return params;
}
/****************************************************************************** TMC_QuickDist ********************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::TMC_QuickDist()
{
//  TMC_QuickDist();
    auto req = GeoCom_Class_Leica::createRequest(2117,"");
    std::vector<std::string> params = GeoCom_Class_Leica::writeAndRead(req);
    return params;
}
/****************************************************************************** TMC_GetCoordinate ****************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::TMC_GetCoordinate(int WaitTime,int mode)
{
//  TMC_GetCoordinate(5,1); WaitTime in ms
    auto req  = GeoCom_Class_Leica::createRequest(2082,std::to_string(WaitTime)+","+std::to_string(mode));
    std::vector<std::string> params = GeoCom_Class_Leica::writeAndRead(req);
    return params;
}
/****************************************************************************** TMC_GetSimpleMea *****************************************************************************************/
std::vector<std::string> GeoCom_Class_Leica::TMC_GetSimpleMea(int WaitTime,int mode)
{
//  TMC_GetSimpleMea(5,1); WaitTime in ms
    auto req  = GeoCom_Class_Leica::createRequest(2108,std::to_string(WaitTime)+","+std::to_string(mode));
    std::vector<std::string> params = GeoCom_Class_Leica::writeAndRead(req);
    return params;
}
/******************************************************************************** CreateRequest ******************************************************************************************/
std::string GeoCom_Class_Leica::createRequest(int cmd,std::string args)
{
    std::string request = "%R1Q,"; // Probar a añadir al principio \n, es decir, quedaría de la forma "\n%R1Q"
    request = request + std::to_string(cmd) + ":" + args + "\r\n";
    return request;
}
/********************************************************************************* AUT_LockIn ********************************************************************************************/
std::string GeoCom_Class_Leica::AUT_LockIn()
{
//  buffer = "%R1Q,9013:\r\n";
    auto req  = GeoCom_Class_Leica::createRequest(9013,"");
    std::vector<std::string> params = GeoCom_Class_Leica::writeAndRead(req);
    return params[0];
}
/****************************************************************************** ConnectSocket ********************************************************************************************/
bool GeoCom_Class_Leica::connectSocket(int _port, char *_IP)
{
    //std::cout << "\nIP direction of server: " << _IP << " with port number: " << _port << std::endl;
    std::cout << "Connecting to TS Leica..." << std::endl;
    return clientSocket.connectToHost(_IP,_port);
}
/****************************************************************************** InitTotalStation *****************************************************************************************/
void GeoCom_Class_Leica::initTotalStation()
{
    GeoCom_Class_Leica::SetPrismType(7); //7 for mini prism and 3 for normal prism
    std::string GRC_OK = GeoCom_Class_Leica::AUT_LockIn();
    while (std::stoi(GRC_OK)!=0){
        std::cout << "Locking for target..." << std::endl;
        GeoCom_Class_Leica::AUT_Search(2,2,0);
        GRC_OK = GeoCom_Class_Leica::AUT_LockIn();
    }
    std::cout << "Target detected. LockIn execution successful" << std::endl;
    //GeoCom_Class_Leica::TMC_SetOrientation(0);
    GeoCom_Class_Leica::TMC_SetEdmMode(9);
    GeoCom_Class_Leica::TMC_DoMeasure(1,1);
//    TMC_GetSimpleMea(5,1);
    GeoCom_Class_Leica::TMC_QuickDist();
    GeoCom_Class_Leica::TMC_GetCoordinate(1000,1);
}
/******************************************************************************* Set Prism Type ******************************************************************************************/
void GeoCom_Class_Leica::SetPrismType(int prism)
{
/*
        buffer = "%R1Q,17008:PrismType\r\n";
        BAP_PRISM_ROUND = 0,        Leica Circular Prism
        BAP_PRISM_MINI = 1,         Leica Mini Prism
        BAP_PRISM_TAPE = 2,         Leica Reflector Tape
        BAP_PRISM_360 = 3,          Leica 360o Prism
        BAP_PRISM_USER1 = 4,        not supported by FlexLine
        BAP_PRISM_USER2 = 5,        not supported by FlexLine
        BAP_PRISM_USER3 = 6,        not supported by FlexLine
        BAP_PRISM_360_MINI = 7,     Leica Mini 360o Prism
        BAP_PRISM_MINI_ZERO = 8,    Leica Mini Zero Prism
        BAP_PRISM_USER = 9          User Defined Prism
        BAP_PRISM_NDS_TAPE = 10     Leica HDS Target
*/
    auto req = GeoCom_Class_Leica::createRequest(17008,std::to_string(prism));
    GeoCom_Class_Leica::writeAndRead(req);

}
/****************************************************************************** Set_Orientation ******************************************************************************************/
void GeoCom_Class_Leica::TMC_SetOrientation(int orientation)
{
//  buffer = "%R1Q,2113:HzOrientation\r\n";
    auto req  = GeoCom_Class_Leica::createRequest(2113,std::to_string(orientation));
    GeoCom_Class_Leica::writeAndRead(req);

}
/******************************************************************************** SetEdmMode *********************************************************************************************/
void GeoCom_Class_Leica::TMC_SetEdmMode(int mode)
{
/*
        buffer = "%R1Q,2020:EdmMode\r\n";
        EDM_MODE_NOT_USED         = 0, // Init value
        EDM_SINGLE_TAPE           = 1, // IR Stan[i]dard Reflector Tape
        EDM_SINGLE_STANDARD       = 2, // IR Standard
        EDM_SINGLE_FAST           = 3, // IR Fast
        EDM_SINGLE_LRANGE         = 4, // LO Standard
        EDM_SINGLE_SRANGE         = 5, // RL Standard
        EDM_CONT_STANDARD         = 6, // Standard repeated measurement
        EDM_CONT_DYNAMIC          = 7, // IR Tacking
        EDM_CONT_REFLESS          = 8, // RL Tracking
        EDM_CONT_FAST             = 9, // Fast repeated measurement
        EDM_AVERAGE_IR            = 10 // IR Average
        EDM_AVERAGE_SR            = 11 // RL Average
        EDM_AVERAGE_LR            = 12 // LO Average
*/
    auto req  = GeoCom_Class_Leica::createRequest(2020,std::to_string(mode));
    GeoCom_Class_Leica::writeAndRead(req);

}
/********************************************************************************* DoMeasure *********************************************************************************************/
void GeoCom_Class_Leica::TMC_DoMeasure(int cmd, int mode)
{
/*
        buffer = "%R1Q,2008:cmd,mode\r\n";
        cmd OPTIONS:
        TMC_STOP          = 0, // Stop measurement program
        TMC_DEF_DIST      = 1, // Default DIST-measurement program
        TMC_CLEAR         = 3, // TMC_STOP and clear data
        TMC_SIGNAL        = 4, // Signal measurement (test function)
        TMC_DO_MEASURE    = 6, // (Re)start measurement task
        TMC_RTRK_DIST     = 8, // Distance-TRK measurement program
        TMC_RED_TRK_DIST  = 10 // Reflectorless tracking
        TMC_FREQUENCY     = 11 // Frequency measurement (test)
        mode OPTIONS:
        TMC_MEA_INC     // encoded as 0
        TMC_AUTO_INC    // encoded as 1
        TMC_PLANE_INC   // encoded as 2
*/
    auto req  = GeoCom_Class_Leica::createRequest(2008,std::to_string(cmd)+","+std::to_string(mode));
    GeoCom_Class_Leica::writeAndRead(req);

}
/******************************************************************************** AUT_Search *********************************************************************************************/
void GeoCom_Class_Leica::AUT_Search(double Hz_Area,double V_Area,bool bDummy)
{
//  AUT_Search(4,4,0);
    auto req  = GeoCom_Class_Leica::createRequest(9029,std::to_string(Hz_Area)+","+std::to_string(V_Area)+","+std::to_string(bDummy));
    GeoCom_Class_Leica::writeAndRead(req);

}
/****************************************************************************** AUT_FineAdjust *******************************************************************************************/
void GeoCom_Class_Leica::AUT_FineAdjust(double dSrchHz, double dSrchV, bool bDummy)
{
//  AUT_FineAdjust(0.1,0.1,0);
    auto req  = GeoCom_Class_Leica::createRequest(9037,std::to_string(dSrchHz)+","+std::to_string(dSrchV)+","+std::to_string(bDummy));
    GeoCom_Class_Leica::writeAndRead(req);

}
/*************************************************************************** AUS_SetUserLockState ****************************************************************************************/
void GeoCom_Class_Leica::AUS_SetUserLockState(bool state)
{
//  buffer = "%R1Q,18007:1\r\n"; state -> OFF = 0; ON = 1;
    auto req  = GeoCom_Class_Leica::createRequest(18007,std::to_string(state));
    GeoCom_Class_Leica::writeAndRead(req);

}
/*****************************************************************************************************************************************************************************************/
