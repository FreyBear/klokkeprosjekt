/*
  KLOKKEPROSJEKT 2026
  ====================
  En smart skole-timeplan klokke med ESP32 og NeoPixel LED-ring
    
  Hver elev/gruppe skal implementere sin egen funksjon.
  Alle funksjoner må følge spesifikasjonene nøyaktig!
*/

#include <WiFi.h> // Bibliotek for WiFi-funksjoner, inkludert i Arduino IDE (Må bruke ESP32 eller ESP8266 for WiFi)
#include <time.h> // Bibliotek for tidsfunksjoner, inkludert i Arduino IDE
#include <sys/time.h> // Bibliotek for timeval og settimeofday (brukes til dummytid/testing)
#include <Adafruit_NeoPixel.h> // Bibliotek for NeoPixel LED-ring, må lastes ned via Library Manager i Arduino IDE

// ========== HARDWARE KONFIGURASJON ==========
#define LED_PIN 25        // Pin for NeoPixel ring
#define NUM_LEDS 77       // Antall LEDs på ringen/stripen (juster dette tallet hvis dere bruker en ring med f.eks. 60 eller 24 LEDs)
#define soundpin 26       // Buzzerpin hvis vil bruke buzzer....

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ========== WIFI KONFIGURASJON ==========
const char* ssid = "Wokwi-GUEST";      // WiFi navn
const char* password = "";               // WiFi passord

// ========== ENUMS OG DATASTRUKTURER ==========
enum Fag {
  INGENTING = 0, FRIMINUTT, ELKRETSER, ENOGSTYR, NORSK, ENGELSK, MATTE, NATURFAG, GYM
};

enum Ukedag {
  SONDAG = 0, MANDAG, TIRSDAG, ONSDAG, TORSDAG, FREDAG, LORDAG
};

struct timePlan {
  Ukedag dag;       // Se enum Ukedag
  byte startTime;   // 0-23
  byte startMinutt; // 0-59
  byte varighet;    // Minutter
  Fag fag;          // Se enum for fag
};

// ========== GLOBALE VARIABLER ==========
timePlan plan[50];     // Array for hele ukens timeplan (Ikke laget enda)
int antallTimer = 2;   // TODO: Oppdater til HØYESTE INDEX + 1 når fyllPlan() er ferdig utfylt (satt til 2 for test-eksempler)

Fag gjeldendeFag = INGENTING; // Hvilket fag vi har nå (bruker enum)
Fag forrigeFag = INGENTING;   // Hvilket fag vi hadde før
int sekunderIgjen = 0;        // Sekunder igjen av gjeldende aktivitet

// Status-flagg for animasjoner
bool nyTime = false;          // True når en ny time starter
bool nyttFriminutt = false;   // True når et nytt friminutt starter
bool ferdigForDagen = false;  // True når skoledagen er ferdig
bool erHelg = false;          // True når det er helg

int melodi[3] = {1, 2, 3};             // Eksempel-toner til melodiSpiller()
int melodi_varighet[3] = {100, 100, 100}; // Eksempel-varigheter (ms) til melodiSpiller()

// ========== FUNKSJONSERKLÆRINGER ==========
// Disse funksjonene må elevene implementere:

// ENKLE FUNKSJONER:
bool sjekkHelg(int ukedag); // Returnerer true hvis ukedag er lørdag/søndag
uint32_t fagFarge(Fag fag); // Returnerer LED-fargen som hører til faget
String fagNavn(Fag fag); // Returnerer navnet på faget som tekst (til Serial Monitor)
void blinkLED(uint32_t farge, int antallBlink); // Blinker hele ringen i en farge et gitt antall ganger
void timeStartAnimasjon(uint32_t fagfarge); // Viser en kort animasjon når en ny time starter
void friminuttAnimasjon(int minutt, int index); // Viser en animasjon i friminuttet
void visMeny(); // Meny hvor brukeren kan aktivere alle funksjonene
void sjekkSerialMeny(); // Sjekker Serial-input og åpner menyen hvis brukeren skriver "meny"

// MIDDELS FUNKSJONER:
void visKlokkevisere(int time, int minutt, int sec); // Tegner time-, minutt- og sekundviser på ringen
void nedtellingBar(int index, int sekunderIgjen, uint32_t fagfarge); // Tegner en nedtellingsbue for gjenværende tid av aktiviteten
void melodiSpiller(int note[], int varighet[], int antallToner); // Spiller av en liste med toner og varigheter på buzzeren
void spillMelodi(int melodiNr); // Spiller lyd/blink for gitt melodi-nummer
void visGjeldendeStatus(int index, int minutt); // Viser animasjon ved hendelser basert på status-flaggene
void ferdigForDagenAnimasjon(); // Viser animasjon når skoledagen er ferdig
void helgeSluttAnimasjon(); // Viser feiringsanimasjon etter siste time på fredag
void handterHelg(int ukedag); // Sjekker om det er helg og oppdaterer flagg/animasjon

// AVANSERTE FUNKSJONER:
void fyllPlan(); // Fyller plan[] med ukens timeplan
int beregnTidIgjen(int index, int time, int minutt, int sekund); // Returnerer sekunder igjen av gjeldende aktivitet
Fag hentGjeldendeFag(int index); // Returnerer faget for gitt indeks i plan[]
bool hentInternetTid(); // Kobler til WiFi/NTP og henter riktig klokkeslett, returnerer true ved suksess
void helgAnimasjon(); // Viser animasjon i helgene
int planIndex(int ukedag, int time, int minutt); // Returnerer indeksen i plan[] for gjeldende time, eller -1
void handterAktivitetsbytte(Fag nyttFag, int ukedag); // Oppdager fagbytte og setter riktige status-flagg/melodi
void settDummyTid(int ukedag, int time, int minutt, int sekund); // Setter ESP32-klokken manuelt til test-tidspunkt

//==================================================================================================================================

void startupAnimasjon() {
  // Enkel oppstart-animasjon
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 50, 100));
    strip.show();
    delay(10);
  }
  strip.clear();
  strip.show();
}

void settDummyTid(int ukedag, int time, int minutt, int sekund) {
  // Setter ESP32-klokken manuelt til et gitt tidspunkt slik at klokken kan tikke og testes uten fungerende WiFi/NTP.
  // Referanseuke i september 2026: SONDAG=6. sept, MANDAG=7. sept, TIRSDAG=8. sept, ONSDAG=9. sept, etc.
  struct tm t = {0};
  t.tm_year = 2026 - 1900;
  t.tm_mon  = 8;          // September (0 = jan, 8 = sep)
  t.tm_mday = 6 + ukedag; // Dag i måneden som matcher ukedagen (SONDAG=0 -> 6. sept)
  t.tm_hour = time;
  t.tm_min  = minutt;
  t.tm_sec  = sekund;
  t.tm_isdst = 1;         // Norsk sommertid

  time_t epoch = mktime(&t);
  struct timeval tv;
  tv.tv_sec = epoch;
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  Serial.println("🕐 KLOKKEPROSJEKT STARTER...");
  
  // Initialiser LED-strip
  strip.begin();
  strip.show();
  startupAnimasjon();
  
  // Koble til WiFi og hent tid
  WiFi.begin(ssid, password); // Start forbindelsen til internett
  if(hentInternetTid()) {
    Serial.println("✅ WiFi og tid OK!");
    blinkLED(strip.Color(0, 255, 0), 2); // Grønn = success
  } else {
    Serial.println("⚠️ WiFi/NTP ikke tilkoblet (eller ikke ferdig implementert).");
    Serial.println("🕒 Setter dummytid: Onsdag kl. 09:30:00 (for at klokken skal tikke og kunne testes)");
    settDummyTid(ONSDAG, 9, 30, 0);
    blinkLED(strip.Color(255, 150, 0), 2); // Oransje/gul = dummytid aktiv
  }
  
  // Lag timeplan
  fyllPlan();
  Serial.println("📅 Timeplan lastet!");
  
  Serial.println("🚀 Klokke klar!");
}

// ========== HOVEDLOOP ==========
void loop() {
  // Hent gjeldende tid
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);

  sjekkSerialMeny();
  handterHelg(timeinfo.tm_wday);

  // Beregn gjeldende fag og tid igjen
  int index_plan = planIndex(timeinfo.tm_wday, timeinfo.tm_hour, timeinfo.tm_min);
  Fag nyttFag = hentGjeldendeFag(index_plan);
  sekunderIgjen = beregnTidIgjen(index_plan, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  handterAktivitetsbytte(nyttFag, timeinfo.tm_wday);
  gjeldendeFag = nyttFag;

  // Sjekk om det skal vises en hendelses-animasjon (ny time, friminutt, ferdig for dagen)
  visGjeldendeStatus(index_plan, timeinfo.tm_min);

  // Normal visning på LED-ringen (nedtelling og klokkevisere)
  strip.clear();
  nedtellingBar(index_plan, sekunderIgjen, fagFarge(gjeldendeFag));
  visKlokkevisere(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  strip.show();

  // Debug info hver 5. sekund
  static unsigned long lastDebug = 0;
  if(millis() - lastDebug > 5000) {
    debugInfo();
    lastDebug = millis();
  }
  
  delay(500); // Oppdater to ganger i sekundet (hvert 500. ms)
}

void debugInfo() {
  // Tips: Utvid gjerne med flere Serial.print()-linjer etter hvert som dere legger til flere variabler (f.eks. erHelg, index_plan, WiFi.status())
  Serial.print("Fag: ");
  Serial.print(fagNavn(gjeldendeFag));
  Serial.print(" (id ");
  Serial.print(gjeldendeFag);
  Serial.print(") | Sekunder igjen: ");
  Serial.print(sekunderIgjen);
  Serial.print(" (ca. ");
  Serial.print(sekunderIgjen / 60);
  Serial.println(" min)");
}

// ========== FUNKSJONSIMPLEMENTASJONER ==========
// ELEVENE MÅ IMPLEMENTERE DISSE FUNKSJONENE:

bool sjekkHelg(int ukedag) {
  // Laget av: 
  // TODO: Returner true hvis det er helg (lørdag eller søndag), ellers false
  // Input: ukedag er et tall der SONDAG=0 ... LORDAG=6 (se enum Ukedag)
  // Tips: Sammenlign ukedag med enum-verdiene SONDAG og LORDAG
  return false; // Placeholder - alltid "ikke helg"
}

//==================================================================================================================================

void handterHelg(int ukedag) {
  // Laget av: 
  // TODO: Bruk sjekkHelg(ukedag) til å avgjøre om det er helg akkurat nå
  // TODO: Oppdater den globale variabelen erHelg, og kall helgAnimasjon() når det er helg
  // Tips: Skriv gjerne ut en Serial-melding kun første gang det blir helg (bruk erHelg til å huske status)
}

//==================================================================================================================================


void visMeny() {
  // Laget av: 
  // TODO: Lag en meny på Serial-monitor der brukeren kan velge å teste de andre funksjonene
  // Tips: Serial.println() for å skrive ut valg, Serial.readStringUntil('\n') for å lese input
  // Tips: input.toInt() gjør om teksten brukeren skriver til et tall du kan bruke i switch/case
  // Tips: Du kan kalle settDummyTid(ONSDAG, 9, 30, 0) som et menyvalg for å simulere/teste ulike dager og tider!
  Serial.println("TODO: Lag menyen din her!");
}

//==================================================================================================================================

void sjekkSerialMeny() {
  // Laget av: 
  // TODO: Sjekk om brukeren har skrevet noe i Serial-monitoren, og åpne menyen med visMeny() hvis teksten er "meny"
  // Tips: Serial.available() forteller om det finnes tekst å lese
  // Tips: Serial.readStringUntil('\n') leser inn teksten, input.trim() fjerner mellomrom/linjeskift
}

//==================================================================================================================================

uint32_t fagFarge(Fag fag) {
  // Laget av: 
  // TODO: Returner en unik LED-farge for hvert fag i enum Fag
  // Tips: strip.Color(r, g, b) tar verdier fra 0-255 for hver fargekanal
  // Tips: Bruk switch/case eller if-else, én farge per fag
  switch (fag) {
    case INGENTING:
      return strip.Color(0, 0, 0); // Eksempel: svart/av
    default:
      return strip.Color(0, 0, 0); // Placeholder - bytt ut med egne farger!
  }
}

//==================================================================================================================================

String fagNavn(Fag fag) {
  // Laget av: 
  // TODO: Returner navnet på faget som lesbar tekst til Serial Monitor
  // Tips: Bruk switch/case akkurat som i fagFarge(), med en tekststreng per fag:
  //       case MATTE: return "Matematikk";
  //       case ENOGSTYR: return "Energi og styresystemer";
  // Tips: Husk et default-case for ukjente fag
  switch (fag) {
    case INGENTING: return "Fri / Ingen time";
    case FRIMINUTT: return "Friminutt";
    default:        return "Fag-id " + String(fag); // Placeholder - fyll inn fagnavn her!
  }
}

//==================================================================================================================================

void blinkLED(uint32_t farge, int antallBlink) {
  // Laget av: 
  // TODO: Blink hele ringen "antallBlink" ganger med fargen "farge"
  // Tips: strip.fill(farge) fyller alle LEDs, strip.clear() slår dem av
  // Tips: Husk strip.show() etter hver endring, og en kort delay() mellom av/på
}

//==================================================================================================================================

void timeStartAnimasjon(uint32_t fagfarge) {
  // Laget av: 
  // TODO: Vis en kort animasjon når en ny time starter, i fagets farge
  // Input: fagfarge er fargen til faget som nettopp startet
  // Tips: F.eks. la fargen "vokse" ut fra ett punkt, eller fyll ringen gradvis
}

//==================================================================================================================================

void friminuttAnimasjon(int minutt, int index){
  // Laget av: 
  // TODO: Vis en animasjon i friminuttet som viser hvor mye tid som er igjen
  // Input: minutt er gjeldende minutt, index er raden i plan[] for friminuttet
  if(index == -1){return;}

  // Tips: plan[index].startMinutt og plan[index].varighet forteller når friminuttet startet/slutter
  // Tips: Bruk map() for å regne om et tidspunkt til en LED-posisjon (0 til NUM_LEDS)
}


//==================================================================================================================================

// Hjelpefunksjoner for klokkevisere (disse er ferdig laget og kan brukes direkte i visKlokkevisere):
int time_viser(int time, int minutt) {
  int t_min = time % 12 * 60;
  int time_pix = ((t_min + minutt) * NUM_LEDS) / 720;
  return time_pix;
}

int minutt_viser(int minutt, int sekund) {
  int min_pix = ((minutt * 60 + sekund) * NUM_LEDS) / 3599;
  return min_pix;
}

int sekund_viser(int sekund) {
  float total = sekund + (millis() % 1000) / 1000.0;
  return (int)((total * NUM_LEDS) / 60);
}

void visKlokkevisere(int time, int minutt, int sec) {
  // Laget av: 
  // TODO: Tegn time-, minutt- og sekundviseren på ringen
  // Input: time (0-23), minutt (0-59), sec (0-59)
  // Tips: Du kan bruke de ferdige hjelpefunksjonene over for å finne LED-indeksene:
  //       int pTime = time_viser(time, minutt);
  //       int pMin  = minutt_viser(minutt, sec);
  //       int pSek  = sekund_viser(sec);
  // Tips: strip.setPixelColor(pixel, farge) tegner én piksel om gangen (f.eks. rød timeviser, blå minuttviser, hvit sekundviser)
}

//==================================================================================================================================

void nedtellingBar(int index, int sekunderIgjen, uint32_t fagfarge) {
  // Laget av: 
  // TODO: Tegn en "bue" av LEDs som viser hvor mye tid som er igjen av gjeldende aktivitet
  // Input: index er raden i plan[], sekunderIgjen er tid igjen, fagfarge er fargen som skal brukes
  strip.clear();
  if(index == -1){return;}

  // Tips: Bruk plan[index].startTime/.startMinutt/.varighet for å finne når aktiviteten startet/slutter
  // Tips: map() kan regne om et tidspunkt (sekunder) til en LED-posisjon (0 til NUM_LEDS)
}

//==================================================================================================================================

void melodiSpiller(int note[], int varighet[], int antallToner) {
  // Laget av: 
  // TODO: Spill av "antallToner" toner fra "note" med tilhørende varighet fra "varighet"
  // Input: note[] er frekvenser (Hz), varighet[] er hvor lenge hver tone skal spilles (ms)
  // Tips: Bruk tone(soundpin, note[i], varighet[i]) for hver tone i en for-løkke (i fra 0 til antallToner-1)
  // Tips: Legg gjerne inn en kort delay() mellom tonene også, slik at de ikke flyter sammen
  // Tips: IKKE bruk sizeof(note) her - arrays som sendes til funksjoner "råtner" til pekere, så det gir feil svar
}


void spillMelodi(int melodiNr) {
  // Laget av: 
  // TODO: Implementer denne funksjonen
  // Input: melodiNr er melodinummer (1=friminutt, 2=time, etc.)
  // Spill en melodi (kan være tom implementasjon hvis ingen buzzer)
  
  // TIPS til implementering - ALTERNATIV 1 (med buzzer):
  // - Bruk tone(soundpin, frekvens, varighet) eller kall melodiSpiller(melodi, melodi_varighet, 3)
  // - Friminutt: Glad melodi (høye toner)
  // - Time: Nøytral tone (middels toner)
  //
  // TIPS til implementering - ALTERNATIV 2 (uten buzzer):
  // - Bruk LED-signaler i stedet for lyd
  // - Friminutt: blinkLED(strip.Color(0, 255, 0), 3) // Grønn
  // - Time: blinkLED(strip.Color(255, 0, 0), 2) // Rød
  // - Lag forskjellige blinkmønstre for hvert melodiNr
  
  // Placeholder - velg alternativ og implementer!
}



//==================================================================================================================================

void ferdigForDagenAnimasjon() {
  // Laget av: 
  // TODO: Vis en rolig animasjon som markerer at skoledagen er ferdig
  // Tips: F.eks. en myk fade inn/ut med strip.fill() og strip.show()
  // Tips: strip.Color(r, g, b) lager fargen, delay() styrer hastigheten
}

//==================================================================================================================================

void helgeSluttAnimasjon() {
  // Laget av: 
  // TODO: Implementer denne funksjonen
  // Vis en spektakulær animasjon når siste time på fredag er ferdig (Maks 30 sekunder)
  
  // TIPS til implementering:
  // - Dette er den store celebrasjonen - vær kreativ og spektakulær!
  // - Kombiner flere effekter: fyrverkeri, regnbuer, puls, rotasjoner
  // - Bruk alle 30 sekunder for en episk opplevelse
  // - Start rolig og bygg opp til klimaks
  // - Eksempel: Fyrverkeri → regnbue → feiring → rolig slutt
  // - Bruk math-funksjoner for smooth overganger
  // - Dette er eleven sin sjanse til å virkelig skinne!
  
  // Placeholder - implementer en episk helgesluttanimasjon!
}

//==================================================================================================================================

void fyllPlan() {
  // Laget av: 
  // TODO: Fyll inn hele ukeplanen med plan[indeks] = {dag, startTime, startMinutt, varighet, fag};
  // Tips: Bruk egne indeks-serier per dag, f.eks. mandag = 0-9, tirsdag = 10-19, osv.
  // Tips: Det er OK å ha hull i listen! Ikke alle indekser trenger å brukes.

  // Eksempel (mandag, første time kl. 08:00 i 45 minutter, i faget MATTE):
  plan[0] = {MANDAG, 8, 0, 45, MATTE};

  // Eksempel (onsdag kl. 09:15 i 60 minutter, i faget ELKRETSER - aktiv under dummytid kl. 09:30):
  plan[1] = {ONSDAG, 9, 15, 60, ELKRETSER};

  // TODO: Legg til resten av timeplanen for hele uken her...
}

//==================================================================================================================================

Fag hentGjeldendeFag(int index) {
  // Laget av: 
  // TODO: Returner faget som står i plan[index]
  // Tips: Husk å sjekke om index er -1 (ingen time nå) og returner INGENTING i så fall
  return INGENTING; // Placeholder
}

//==================================================================================================================================

int beregnTidIgjen(int index, int time, int minutt, int sekund) {
  // Laget av: 
  // TODO: Regn ut hvor mange sekunder som er igjen av aktiviteten i plan[index]
  // Input: index er raden i plan[], time/minutt/sekund er klokkeslettet akkurat nå
  if (index == -1) { return 0; }

  // Tips: Gjør om både "nå" og "når timen slutter" til sekunder fra midnatt og ta differansen:
  //       int naSek = time * 3600 + minutt * 60 + sekund;
  //       int sluttSek = plan[index].startTime * 3600 + (plan[index].startMinutt + plan[index].varighet) * 60;
  //       return max(0, sluttSek - naSek);
  return 0; // Placeholder
}

//==================================================================================================================================

int planIndex(int ukedag, int time, int minutt) {
  // Laget av: 
  // TODO: Finn og returner indeksen i plan[] som matcher gjeldende ukedag/time/minutt
  // Input: ukedag (se enum Ukedag), time (0-23), minutt (0-59)
  // Output: Indeksen (0-49) til riktig rad i plan[], eller -1 hvis ingen time pågår nå
  // Tips: Gjør om klokkeslettet til minutter fra midnatt: int naMin = time * 60 + minutt;
  // Tips: Løp gjennom plan[] (fra 0 til antallTimer - 1). For hver rad der plan[i].dag == ukedag og plan[i].fag != INGENTING:
  //       int startMin = plan[i].startTime * 60 + plan[i].startMinutt;
  //       int sluttMin = startMin + plan[i].varighet;
  //       Sjekk om naMin er fra og med startMin og mindre enn sluttMin. Returner da i!
  return -1; // Placeholder - "ingen time akkurat nå"
}

//==================================================================================================================================

bool hentInternetTid() {
  // Laget av: 
  // TODO: Koble til WiFi og hent riktig klokkeslett fra internett (NTP)
  // Tips: WiFi bruker litt tid på å koble til. Vent i en while-løkke med teller/timeout:
  //       int forsok = 0;
  //       while (WiFi.status() != WL_CONNECTED && forsok < 20) {
  //         delay(500);
  //         forsok++;
  //       }
  //       if (WiFi.status() != WL_CONNECTED) return false;
  // Tips: For norsk tid (GMT+1 og 1 time sommertid):
  //       configTime(3600, 3600, "pool.ntp.org");
  // Tips: Verifiser at tiden er synkronisert med getLocalTime():
  //       struct tm t;
  //       return getLocalTime(&t);
  return false; // Placeholder - "tid ikke hentet"
}

//==================================================================================================================================

void helgAnimasjon() {
  // Laget av: 
  // TODO: Implementer denne funksjonen
  // Lag en kul animasjon som vises i helgene
  
  // TIPS til implementering - Vær kreativ! Dette er din sjanse til å skinne:
  // - Regnbue-effekter: Bruk forskjellige farger som roterer
  // - Bouncing balls: Simuler en ball som spretter
  // - Fade-effekter: Fade inn/ut med forskjellige farger
  // - Roterende mønstre: Roter et mønster rundt ringen
  // - Stjernehimmel: Tilfeldige LEDs som blinker som stjerner
  // - Fyrverk-effekt: Eksplosjoner av farger
  // - Bruk math-funksjoner som sin(), cos() for smooth animasjoner
  // - Kombiner forskjellige effekter!
  // - Husk: ikke bruk for lange delays (animasjonen kalles hver loop)
  
  // Placeholder - vær kreativ og implementer din egen kule animasjon!
}

//==================================================================================================================================

void handterAktivitetsbytte(Fag nyttFag, int ukedag) {
  // Laget av: 
  // TODO: Sjekk om nyttFag er forskjellig fra den globale variabelen forrigeFag (da har vi byttet aktivitet)
  // TODO: Sett riktig status-flagg (ferdigForDagen, nyttFriminutt eller nyTime) og spill riktig melodi med spillMelodi()
  // Tips: Hvis nyttFag == INGENTING og forrigeFag != INGENTING: skoledagen er slutt!
  //       - Hvis ukedag == FREDAG: kall helgeSluttAnimasjon()
  //       - Ellers: sett ferdigForDagen = true
  // Tips: Hvis nyttFag == FRIMINUTT: sett nyttFriminutt = true og kall spillMelodi(1)
  // Tips: Hvis nyttFag != INGENTING og nyttFag != FRIMINUTT: sett nyTime = true og kall spillMelodi(2)
  // TODO: Husk å oppdatere forrigeFag = nyttFag til slutt!
}

//==================================================================================================================================

void visGjeldendeStatus(int index, int minutt) {
  // Laget av: 
  // TODO: Sjekk status-flaggene (ferdigForDagen, nyttFriminutt, nyTime) og vis riktig animasjon
  // Tips: Sjekk flaggene i rekkefølge med if / else if:
  //       - Hvis ferdigForDagen: kall ferdigForDagenAnimasjon() og nullstill flagget (ferdigForDagen = false;)
  //       - Hvis nyttFriminutt:  kall friminuttAnimasjon(minutt, index) og nullstill flagget (nyttFriminutt = false;)
  //       - Hvis nyTime:         kall timeStartAnimasjon(fagFarge(gjeldendeFag)) og nullstill flagget (nyTime = false;)
}



