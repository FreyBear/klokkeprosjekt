# Elevoppgaver – Klokkeprosjekt 2026

Dere skal sammen bygge ferdig en smart skole-timeplan-klokke med en NeoPixel LED-ring. Koden i `sketch/sketch.ino` inneholder alle funksjonene dere trenger, men de fleste er tomme skjeletter merket med `// Laget av:` og `// TODO:`. Hver elev/gruppe velger én eller flere funksjoner og implementerer dem.

## Spilleregler

1. Velg en funksjon fra listen under som ingen andre i klassen har tatt.
2. Skriv navnet ditt i `// Laget av: <Fornavn>` inne i funksjonen.
3. Legg til dokumentasjonskommentaren beskrevet under, **over** funksjonen.
4. Ikke endre funksjonens navn, parametere eller returtype — resten av koden er avhengig av at signaturen er uendret.
5. **Test funksjonen din før du leverer!** Du kan teste den via Serial-monitoren, eller legge til en midlertidig testlinje nederst i `setup()` for å se at den virker (husk å fjerne testlinjen før du committer):
   - For `sjekkHelg`: `Serial.println(sjekkHelg(LORDAG) ? "OK: Helg!" : "Feil");`
   - For `fagNavn`: `Serial.println("Test fagNavn: " + fagNavn(MATTE));`
   - For `fagFarge`: `strip.fill(fagFarge(MATTE)); strip.show();`
   - For animasjoner/visere: kall funksjonen din direkte nederst i `setup()` og se resultatet på ringen!

---

## Dokumentasjonsstandard

Over **hver** funksjon dere lager, skal det stå en kommentarblokk som følger denne malen:

```cpp
/*
 * Funksjon:     <navn på funksjonen>
 * Laget av:     <Fornavn>
 *
 * Hva gjør den?
 *   <1-3 setninger som forklarer hva funksjonen gjør, med egne ord>
 *
 * Parametere:
 *   - <navn> (<type>): <hva parameteren betyr / brukes til>
 *   (skriv "Ingen" hvis funksjonen ikke tar imot noen parametere)
 *
 * Returverdi:
 *   - <type>: <hva som returneres og hva det betyr>
 *   (skriv "Ingen (void)" hvis funksjonen ikke returnerer noe)
 *
 * Refleksjon:
 *   - <Hvorfor løste du det på denne måten? Var det noe som var vanskelig
 *     eller lurt å tenke på? Er det noe du ville gjort annerledes?>
 */
```

Poenget med dokumentasjonen er todelt: den skal gjøre det lett for andre (og deg selv) å forstå hva funksjonen gjør uten å lese hele koden, **og** den skal tvinge deg til å reflektere over løsningen din i stedet for bare å skrive kode og gå videre.

### Eksempel: `uint32_t fagFarge(Fag fag)`

Denne funksjonen tar imot ett fag og returnerer en LED-farge. Slik kan dokumentasjonen se ut når den er korrekt fylt ut:

```cpp
/*
 * Funksjon:     fagFarge
 * Laget av:     Kari
 *
 * Hva gjør den?
 *   Slår opp hvilken farge LED-ringen skal lyse i basert på hvilket fag
 *   som er sendt inn, ved hjelp av en switch-setning på enum-verdien.
 *
 * Parametere:
 *   - fag (Fag): Hvilket skolefag det skal hentes en farge for (se enum Fag
 *     øverst i filen, f.eks. MATTE, NORSK, FRIMINUTT osv.)
 *
 * Returverdi:
 *   - uint32_t: En fargeverdi laget med strip.Color(r, g, b) som kan sendes
 *     rett til strip.setPixelColor()/strip.fill(). Returnerer svart (av)
 *     dersom faget ikke er gjenkjent.
 *
 * Refleksjon:
 *   - Jeg valgte kalde farger (blå/grønn) til teorifag og en varm farge til
 *     gym, slik at det er lett å skille dem fra hverandre på avstand.
 *   - Jeg måtte huske på default-caset i switch-en, ellers ville funksjonen
 *     ikke returnere noe for et ukjent fag.
 */
uint32_t fagFarge(Fag fag) {
  ...
}
```

Bruk denne malen på **alle** funksjonene dere implementerer, ikke bare `fagFarge`.

---

## Funksjonsoversikt

| Vanskelighetsgrad | Funksjon |
|---|---|
| Enkel | `sjekkHelg`, `fagFarge`, `fagNavn`, `blinkLED`, `timeStartAnimasjon`, `friminuttAnimasjon`, `visMeny`, `sjekkSerialMeny` |
| Middels | `visKlokkevisere`, `nedtellingBar`, `melodiSpiller`, `spillMelodi`, `visGjeldendeStatus`, `ferdigForDagenAnimasjon`, `helgeSluttAnimasjon`, `handterHelg` |
| Avansert | `fyllPlan`, `beregnTidIgjen`, `hentGjeldendeFag`, `hentInternetTid`, `helgAnimasjon`, `planIndex`, `handterAktivitetsbytte` |
| Hardware (samarbeid, ikke koding) | Kabling og lodding, 3D-printet ramme — se [Hardware-oppgaver](#hardware-oppgaver-krever-samarbeid) nederst |

---

## Enkle funksjoner

### `bool sjekkHelg(int ukedag)`
Avgjør om en gitt ukedag er en helgedag.
- **Parameter:** `ukedag` (int) – tallverdi for ukedagen, se `enum Ukedag` (SONDAG=0 ... LORDAG=6).
- **Returverdi:** `bool` – `true` hvis det er lørdag eller søndag, ellers `false`.
- **Hint:** Sammenlign `ukedag` med enum-verdiene `SONDAG` og `LORDAG` i stedet for tallene 0/6 direkte — det gjør koden lettere å lese.

### `uint32_t fagFarge(Fag fag)`
Se det utfylte eksempelet over. Returnerer LED-fargen som hører til et gitt fag.
- **Parameter:** `fag` (Fag) – hvilket fag (se `enum Fag`).
- **Returverdi:** `uint32_t` – fargeverdi laget med `strip.Color(r, g, b)`.
- **Hint:** Bruk `switch`/`case` med én farge per fag, og husk et `default`-case.

### `String fagNavn(Fag fag)`
Gjør om et fag fra `enum Fag` til en lesbar tekst som skrives ut på Serial-monitoren.
- **Parameter:** `fag` (Fag) – hvilket fag det gjelder (f.eks. `MATTE`, `ENOGSTYR`, `FRIMINUTT`).
- **Returverdi:** `String` – navnet på faget som tekst (f.eks. `"Matematikk"`, `"Energi og styresystemer"`, `"Friminutt"`).
- **Hint:** Bruk `switch`/`case` på samme måte som i `fagFarge()`, men returner tekst i stedet for farge. Husk `default: return "Fri / Ukjent";`.

### `void blinkLED(uint32_t farge, int antallBlink)`
Blinker hele LED-ringen i en gitt farge et antall ganger.
- **Parametere:** `farge` (uint32_t) – fargen som skal blinkes, `antallBlink` (int) – hvor mange ganger den skal blinke.
- **Returverdi:** Ingen (void).
- **Hint:** `strip.fill(farge)` + `strip.show()` for å slå på, `strip.clear()` + `strip.show()` for å slå av, med en `delay()` mellom hver.

### `void timeStartAnimasjon(uint32_t fagfarge)`
Viser en kort animasjon når en ny time starter.
- **Parameter:** `fagfarge` (uint32_t) – fargen til faget som nettopp startet.
- **Returverdi:** Ingen (void).
- **Hint:** Prøv f.eks. å la fargen "vokse" ut fra ett punkt på ringen, eller fylle ringen gradvis.

### `void friminuttAnimasjon(int minutt, int index)`
Viser en animasjon i friminuttet som gjerne bør vise hvor mye tid som er igjen.
- **Parametere:** `minutt` (int) – gjeldende minutt, `index` (int) – raden i `plan[]` for friminuttet (kan være `-1` hvis det ikke er friminutt).
- **Returverdi:** Ingen (void).
- **Hint:** `plan[index].startMinutt` og `plan[index].varighet` forteller når friminuttet startet/slutter. Husk å håndtere `index == -1`.

### `void visMeny()`
Viser en meny på Serial-monitoren der man kan velge å teste de andre funksjonene.
- **Parametere:** Ingen.
- **Returverdi:** Ingen (void).
- **Hint:** `Serial.println()` for menyvalg, `Serial.readStringUntil('\n')` + `input.toInt()` for å lese hva brukeren velger, og `switch`/`case` for å kalle riktig funksjon. Du kan også bruke den ferdige hjelpefunksjonen `settDummyTid(ukedag, time, minutt, sekund)` i et menyvalg for å simulere ulike dager og tider (f.eks. onsdag 09:30, fredag 13:59 eller lørdag).

### `void sjekkSerialMeny()`
Sjekker om brukeren har skrevet noe på Serial-monitoren, og åpner menyen hvis teksten er `"meny"`.
- **Parametere:** Ingen.
- **Returverdi:** Ingen (void).
- **Hint:** `Serial.available()` forteller om det finnes tekst å lese. Husk `input.trim()` for å fjerne mellomrom/linjeskift før du sammenligner strengen.

---

## Middels funksjoner

### `void visKlokkevisere(int time, int minutt, int sec)`
Tegner time-, minutt- og sekundviser på LED-ringen, som en vanlig klokke.
- **Parametere:** `time` (0-23), `minutt` (0-59), `sec` (0-59).
- **Returverdi:** Ingen (void).
- **Hint:** En hel runde rundt ringen (`NUM_LEDS` piksler) tilsvarer 12 timer for timeviseren, og 60 minutter/sekunder for de andre viserne. I koden ligger hjelpefunksjonene `time_viser()`, `minutt_viser()` og `sekund_viser()` ferdig laget rett over funksjonen – du kan kalle disse direkte for å finne pikselposisjonen til hver viser, og så sette farger med `strip.setPixelColor(pos, farge)`.

### `void nedtellingBar(int index, int sekunderIgjen, uint32_t fagfarge)`
Tegner en "bue" av LED-er som viser hvor mye tid som er igjen av den aktiviteten som pågår nå.
- **Parametere:** `index` (int) – raden i `plan[]`, `sekunderIgjen` (int) – sekunder igjen, `fagfarge` (uint32_t) – fargen buen skal ha.
- **Returverdi:** Ingen (void).
- **Hint:** Bruk `plan[index].startTime`/`.startMinutt`/`.varighet` til å finne når aktiviteten startet/slutter, og `map()` for å regne om tid til LED-posisjon. Husk å håndtere `index == -1`.

### `void melodiSpiller(int note[], int varighet[], int antallToner)`
Spiller av en liste med toner (frekvenser) og tilhørende varigheter på buzzeren.
- **Parametere:** `note[]` – frekvenser i Hz, `varighet[]` – varighet per tone i ms, `antallToner` (int) – hvor mange toner arrayene inneholder.
- **Returverdi:** Ingen (void).
- **Hint:** Bruk `tone(soundpin, note[i], varighet[i])` i en for-løkke fra `0` til `antallToner - 1`. **Ikke** bruk `sizeof(note)` — arrays som sendes til funksjoner "råtner" til pekere, så det gir feil svar. Det er derfor `antallToner` sendes med som egen parameter.

### `void spillMelodi(int melodiNr)`
Spiller riktig melodi/varsel basert på et melodi-nummer (1 = friminutt, 2 = time-start, osv.).
- **Parameter:** `melodiNr` (int) – melodinummer (f.eks. 1 = friminutt, 2 = time-start).
- **Returverdi:** Ingen (void).
- **Hint:** Har dere buzzer? Bruk `tone(soundpin, frekvens, varighet)` eller spill av eksempel-tonene med `melodiSpiller(melodi, melodi_varighet, 3)`. Ingen buzzer? Bruk LED-blink i stedet, f.eks. `blinkLED(strip.Color(0,255,0), 3)`.

### `void visGjeldendeStatus(int index, int minutt)`
Sjekker statusflaggene og starter riktig animasjon dersom det har skjedd en hendelse (skoledag over, friminutt eller ny time).
- **Parametere:** `index` (int) – raden i `plan[]` for aktiviteten, `minutt` (int) – gjeldende minutt.
- **Returverdi:** Ingen (void).
- **Hint:** Sjekk de globale statusflaggene i rekkefølge med `if` / `else if`:
  - Hvis `ferdigForDagen`: vis `ferdigForDagenAnimasjon()` og nullstill flagget (`ferdigForDagen = false;`).
  - Hvis `nyttFriminutt`: vis `friminuttAnimasjon(minutt, index)` og nullstill flagget (`nyttFriminutt = false;`).
  - Hvis `nyTime`: vis `timeStartAnimasjon(fagFarge(gjeldendeFag))` og nullstill flagget (`nyTime = false;`).

### `void ferdigForDagenAnimasjon()`
Viser en rolig animasjon som markerer at skoledagen er over.
- **Parametere:** Ingen.
- **Returverdi:** Ingen (void).
- **Hint:** En myk fade inn/ut med `strip.fill()` + `strip.show()` er en god start.

### `void helgeSluttAnimasjon()`
Viser en spektakulær feiringsanimasjon etter siste time på fredag (maks 30 sekunder).
- **Parametere:** Ingen.
- **Returverdi:** Ingen (void).
- **Hint:** Vær kreativ! Kombiner flere effekter (fyrverkeri, regnbue, puls, rotasjon). `sin()`/`cos()` er nyttige for myke overganger. Ikke bruk for lange `delay()`-kall om gangen.

### `void handterHelg(int ukedag)`
Sjekker om det er helg akkurat nå, og håndterer statusflagget og animasjonen for det.
- **Parameter:** `ukedag` (int).
- **Returverdi:** Ingen (void).
- **Hint:** Bruk `sjekkHelg(ukedag)`, oppdater den globale variabelen `erHelg`, og kall `helgAnimasjon()` når det er helg. Vurder å bare skrive en Serial-melding *første* gang det blir helg, ikke hver eneste loop.

---

## Avanserte funksjoner

### `void fyllPlan()`
Fyller `plan[]`-arrayet med hele ukens timeplan.
- **Parametere:** Ingen.
- **Returverdi:** Ingen (void).
- **Hint:** Bruk egne indeks-serier per dag (f.eks. mandag = 0-9, tirsdag = 10-19). Det er lov å ha "hull" i listen. Husk å oppdatere `antallTimer` når planen er fullført.

### `int beregnTidIgjen(int index, int time, int minutt, int sekund)`
Regner ut hvor mange sekunder som er igjen av aktiviteten i `plan[index]`.
- **Parametere:** `index` (int) – raden i `plan[]`, `time` (0-23), `minutt` (0-59), `sekund` (0-59) – klokkeslettet akkurat nå.
- **Returverdi:** `int` – antall sekunder igjen (0 hvis `index == -1`).
- **Hint:** Gjør om både "nå" og "når aktiviteten slutter" til sekunder fra midnatt, og ta differansen (`sluttSek - naSek`). `plan[index].startTime`, `.startMinutt` og `.varighet` gir deg sluttidspunktet.

### `Fag hentGjeldendeFag(int index)`
Slår opp hvilket fag som pågår, ut fra en indeks i `plan[]`.
- **Parameter:** `index` (int).
- **Returverdi:** `Fag` – faget i `plan[index]`, eller `INGENTING` hvis `index == -1`.
- **Hint:** Dette er en enkel oppslagsfunksjon — pass på grensetilfellet med `index == -1`.

### `bool hentInternetTid()`
Kobler til WiFi og henter riktig klokkeslett fra internett (NTP).
- **Parametere:** Ingen.
- **Returverdi:** `bool` – `true` hvis tiden ble hentet, `false` ved feil.
- **Hint:** WiFi bruker 2-5 sekunder på å koble til, så du må vente i en `while (WiFi.status() != WL_CONNECTED)`-løkke med timeout (f.eks. maks 20 forsøk med `delay(500)`). For norsk tidssone (GMT+1 og 1 time sommertid), bruk `configTime(3600, 3600, "pool.ntp.org")`. Sjekk deretter om tiden er gyldig med `getLocalTime(&timeinfo)`.

### `void helgAnimasjon()`
Viser en kul animasjon som spilles gjentatte ganger i helgene.
- **Parametere:** Ingen.
- **Returverdi:** Ingen (void).
- **Hint:** Regnbue-effekter, "bouncing balls", stjernehimmel eller roterende mønstre er gode utgangspunkt. Denne kalles hver loop, så unngå lange `delay()`-kall.

### `int planIndex(int ukedag, int time, int minutt)`
Finner hvilken rad i `plan[]` som matcher gjeldende ukedag/klokkeslett.
- **Parametere:** `ukedag` (int), `time` (0-23), `minutt` (0-59).
- **Returverdi:** `int` – indeksen (0-49) til riktig rad, eller `-1` hvis ingen aktivitet pågår nå.
- **Hint:** Gjør om nåværende tid til minutter fra midnatt: `int naMin = time * 60 + minutt`. Gå gjennom `plan[]` med en for-løkke fra `0` til `antallTimer - 1`. For hver rad der `plan[i].dag == ukedag` og `plan[i].fag != INGENTING`: regn ut `startMin = startTime * 60 + startMinutt` og `sluttMin = startMin + varighet`. Hvis `naMin` er mellom `startMin` og `sluttMin`, returner indeksen `i`.

### `void handterAktivitetsbytte(Fag nyttFag, int ukedag)`
Oppdager om faget/aktiviteten har endret seg siden forrige gang, og setter riktige statusflagg.
- **Parametere:** `nyttFag` (Fag) – faget som gjelder nå, `ukedag` (int).
- **Returverdi:** Ingen (void).
- **Hint:** Sammenlign `nyttFag` med den globale `forrigeFag`.
  - Hvis `nyttFag == INGENTING` og `forrigeFag != INGENTING`, er skoledagen over! Hvis det er fredag (`ukedag == FREDAG`), kall `helgeSluttAnimasjon()`, ellers sett `ferdigForDagen = true`.
  - Hvis `nyttFag == FRIMINUTT`, sett `nyttFriminutt = true` og kall `spillMelodi(1)`.
  - Hvis `nyttFag != INGENTING` og `nyttFag != FRIMINUTT`, sett `nyTime = true` og kall `spillMelodi(2)`.
  - Husk å oppdatere `forrigeFag = nyttFag` til slutt!

---

## Hardware-oppgaver (krever samarbeid)

I tillegg til kodefunksjonene over finnes det to fysiske oppgaver som noen i klassen bør ta ansvar for. De krever ikke koding, men er like viktige for at klokken skal fungere og se bra ut — og de er for store til å gjøre alene, så gå sammen i en gruppe på 2-4.

### 🔌 Kabling og lodding

**Hva skal gjøres?**
1. Lodd ledninger fast på riktig pad/pinne på LED-ringen (DIN, VCC, GND) slik at den kan kobles til ESP32-en med dupontledninger.
2. Koble ESP32-en til LED-ringen (og buzzer, hvis dere har en) med dupontledninger, i tråd med koblingstabellen i [README.md](README.md#hardware-tilkobling).
3. Last opp koden og bekreft at LED-ringen faktisk lyser riktig — det er den endelige testen på at loddingen er god.

**Hvorfor er dette viktig?**
Uansett hvor god koden er, fungerer ikke klokken uten en pålitelig fysisk tilkobling. Dette er en praktisk øvelse i å lese en koblingstabell og omsette den til ekte hardware — en ferdighet som er like nyttig som programmering, og som gir umiddelbar, synlig tilbakemelding når noe er feil (eller riktig!).

**Sikkerhet og hint:**
- Loddebolten blir svært varm (250–350 °C) — bruk loddestativ, ikke ta på spissen/den varme delen, og jobb på et ikke-brennbart underlag.
- Jobb gjerne to og to: én holder og varmer, én mater loddetinn.
- Dobbeltsjekk polaritet (VCC/GND) *før* dere kobler til strøm — feilkobling kan ødelegge LED-ringen eller ESP32-en permanent.
- Test loddepunktene med et multimeter (kontinuitetstest) før strøm kobles til, for å avsløre kalde loddepunkter eller utilsiktede kortslutninger.

**Dokumentasjon:** Ta et bilde av den ferdige kablingen/loddingen og legg det ved i PR-en (f.eks. i en `dokumentasjon/hardware/`-mappe), sammen med en kort refleksjon: Hva var vanskelig? Hvordan testet dere at koblingen fungerte?

### 🖨️ 3D-printet ramme/kabinett

**Hva skal gjøres?**
Design og skriv ut (eller lasérkutt, hvis 3D-printer ikke er tilgjengelig) en ramme som LED-ringen monteres i. Rammen må oppfylle disse kravene:

1. **Reflekterende yttervegg:** LED-ene på ringen stråler i utgangspunktet rett utover og til siden. Rammen bør derfor ha en vegg/kant utenfor ringen som reflekterer lyset videre utover — bruk gjerne hvit/blank plastfarge, og vinkle veggen slik at lyset kastes utover i stedet for å forsvinne til værs.
2. **Skillevegger mellom hver LED:** Lag en liten skillevegg mellom hver enkelt LED (som tynne "kile"-formede rom rundt hver LED, litt som et solur inndelt i skiver), slik at lyset fra én LED ikke blander seg med naboene. Det gjør det mye lettere å se nøyaktig hvilken LED som lyser — avgjørende for f.eks. klokkeviserne og nedtellingsbaren i koden.
3. Rammen må ha plass til ESP32 og eventuell buzzer, og gjerne en åpning for USB-kabelen (strøm/opplasting).

**Hvorfor er dette viktig?**
Dette er en øvelse i produktdesign og fysikk: dere må måle det fysiske utstyret (antall LED-er, ringens diameter, avstanden mellom hver LED), lage en 3D-modell som passer nøyaktig, og tenke på hvordan lys sprer seg og reflekteres — ikke bare hvordan koden styrer fargene.

**Hint:**
- Start med å måle ringens ytre/indre diameter. Finn avstanden mellom hver LED («pitch») ved å dele ringens omkrets på antall LED-er (`NUM_LEDS` i koden).
- Tenk på formen som et solur delt inn i like mange sektorer som det er LED-er — hver LED får sin egen kile-formede "lomme".
- Gratis CAD-verktøy: Tinkercad (nybegynnervennlig, nettbasert) eller Fusion 360 (mer avansert, gratis for studenter).
- Iterér! Print gjerne en liten testbit (f.eks. 1/8 av ringen) først for å sjekke mål og lysspredning, før dere printer hele rammen.

**Dokumentasjon:** Legg ved et skjermbilde av 3D-modellen og et bilde av den ferdig monterte rammen, sammen med en kort refleksjon: Hvilke valg tok dere for å spre lyset godt utover og skille LED-ene fra hverandre? Hva ville dere gjort annerledes neste gang?
