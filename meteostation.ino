#include <EtherCard.h>
#include <DHT.h>

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x33 };

byte Ethernet::buffer[700];
BufferFiller bfill;

#define DHTPIN 3
#define DHTTYPE DHT11
float rh, rt; 

DHT dht(DHTPIN, DHTTYPE);

void setup ()
{
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
  Serial.println( "Failed to access Ethernet controller");
  ether.dhcpSetup();
  Serial.begin(57600);
  dht.begin();
}

static word homePage(int h, int t)
{
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='5'/>"
    "<br>"
    "<h2>Humidity:<br> $D </h2>"
    "<h2>Temperature:<br> $D </h2>"
   ), h, t);   
  return bfill.position();
}


void loop ()
{    
  ether.packetLoop(ether.packetReceive());

  rh = dht.readHumidity();
  rt = dht.readTemperature();
    
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if (isnan(rt) || isnan(rh)) {
    Serial.println("Failed to read from DHT");
  }
  
  if (pos)
   ether.httpServerReply(homePage(rh, rt));
}
