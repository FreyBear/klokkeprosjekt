# Klokkeprosjekt 2026

En smart skole-timeplan-klokke bygget med ESP32 og en NeoPixel LED-ring. Prosjektet er et samarbeid — hver elev/gruppe implementerer én eller flere funksjoner, og alt samles i dette **repoet** (den delte prosjektmappen på GitHub, kort for "repository") gjennom **pull requests** (forkortet PR — en forespørsel om å få endringene dine inn i hovedkoden).

> **Er du en lærer som vil bruke dette opplegget i egen klasse?** Se **[GITHUB_OPPSETT.md](GITHUB_OPPSETT.md)**, steg 0, for hvordan du kopierer repoet til din egen GitHub-organisasjon.

## Kom i gang

1. Åpne `sketch/sketch.ino` i Arduino IDE (åpne selve `sketch.ino`-fila, ikke bare mappen).
2. Installer nødvendig bibliotek: søk på **"Adafruit NeoPixel"** i Library Manager (Ctrl+Shift+I) og installer det.
3. Sørg for at ESP32-kortpakken er installert (Tools → Board → Boards Manager → søk "esp32" av Espressif Systems), og velg riktig ESP32-kort under Tools → Board. Vi bruker den som heter "DOIT ESP32 DEVKIT V1"
4. `WiFi.h`, `time.h` og `<sys/time.h>` følger automatisk med og trenger ingen installasjon.

## Hardware-tilkobling

Koble ESP32 til NeoPixel-ringen og eventuell buzzer slik:

| Signal | NeoPixel / Buzzer Pin | ESP32 Pin | Kommentar |
|---|---|---|---|
| **Spenning** | 5V / VCC | **5V / VIN** | Strømforsyning til LED-ene |
| **Jord** | GND | **GND** | Felles jord |
| **LED-data** | DIN (Data In) | **GPIO 25** | Styresignal til ringen (`LED_PIN`) |
| **Buzzer (+)** | Pluss (+) | **GPIO 26** | *(Valgfritt)* Lydsignal (`soundpin`) |
| **Buzzer (-)** | Minus (-) | **GND** | Felles jord |

> **Tips:** Har dere ikke buzzer koblet til? Ingen fare — funksjonen `spillMelodi` kan fint blinke LED-ringen med farger i stedet for å spille lyd!

## Simulering med Wokwi & VS Code (Valgfritt)

Vil du teste koden direkte i VS Code uten fysisk ESP32/LED-ring? Se **[WOKWI_GUIDE.md](WOKWI_GUIDE.md)** for hvordan du setter opp Wokwi Simulator med Arduino CLI.

## Hva skal jeg gjøre?

Se **[ELEVOPPGAVER.md](ELEVOPPGAVER.md)** for:
- Full oversikt over alle funksjonene som skal implementeres (enkel/middels/avansert)
- Hvilken dokumentasjonsmal du skal bruke over funksjonen din
- Beskrivelse, parametere, returverdi og hint for hver funksjon

Velg én funksjon som ingen andre har tatt, og opprett et **issue** for den på GitHub (**Issues → New issue → Velg en funksjon**) — det er slik hele klassen holder oversikt over hvem som har tatt hva.

> **Lærer:** se [LARERPROSEDYRE.md](LARERPROSEDYRE.md) for hvordan pull requests skal gjennomgås og merges, og [GITHUB_OPPSETT.md](GITHUB_OPPSETT.md) for engangs-oppsettet av organisasjonen.

---

## Slik leverer du: GitHub Desktop + Arduino IDE

Du trenger [GitHub Desktop](https://desktop.github.com/) installert og en GitHub-konto. Du skal **ikke** jobbe direkte i `main` (hovedversjonen av koden — den som til slutt skal fungere for alle) — alt du gjør skjer i din egen **branch** (en løsrevet kopi av koden der du trygt kan gjøre endringer uten å påvirke andre), og læreren godkjenner og slår sammen (**merger** — kombinerer endringene dine inn i `main`) arbeidet ditt etterpå.

### 1. Klon repoet (kun første gang)

Å **klone** betyr å laste ned en komplett kopi av prosjektet (med hele historikken) til din egen PC, slik at du kan jobbe med det lokalt.

1. Åpne GitHub Desktop.
2. **File → Clone Repository**.
3. Velg dette prosjektet fra listen (eller lim inn URL-en under fanen "URL"), og velg hvor på PC-en det skal lagres.
4. Trykk **Clone**.

### 2. Hent siste versjon av main

Hver gang du starter en ny økt: åpne GitHub Desktop, sjekk at du står på branchen `main` (øverst i midten), og trykk **Fetch origin** (sjekker om det finnes nye endringer på GitHub) → **Pull origin** (laster faktisk ned og legger inn de endringene lokalt). Da får du med deg alt andre har levert.

### 3. Lag din egen branch

1. Øverst i GitHub Desktop, trykk på branch-menyen (der det står `main`).
2. Trykk **New branch**.
3. Gi den et navn som viser hva du jobber med, f.eks. `funksjon-fagFarge` eller `kari-fagFarge`.
4. Trykk **Create branch** — nå er du automatisk byttet over til den nye branchen.

### 4. Implementer funksjonen din i Arduino IDE

- Åpne `sketch/sketch.ino` fra mappen du klonet, i Arduino IDE.
- Endre **kun** koden inni funksjonen(e) du har tatt ansvar for.
- Ikke endre navn, parametere eller returtype på funksjonen.
- Legg til `// Laget av: <Fornavn>` og dokumentasjonskommentaren beskrevet i [ELEVOPPGAVER.md](ELEVOPPGAVER.md).
- Lagre filen (Ctrl+S), og test gjerne at koden kompilerer/laster opp riktig.

### 5. Commit endringene dine i GitHub Desktop

En **commit** er et lagringspunkt/øyeblikksbilde av endringene dine, med en kort melding som forklarer hva du gjorde. Du kan gjøre flere commits etter hverandre mens du jobber.

1. Bytt tilbake til GitHub Desktop — den viser nå automatisk hvilke linjer du har endret.
2. Se gjennom endringene i midten av skjermen (grønt = lagt til, rødt = fjernet). Sjekk at det kun er din egen funksjon som er endret.
3. Skriv en kort commit-melding nederst til venstre, f.eks. "Implementer fagFarge()".
4. Trykk **Commit to \<din branch\>**.

### 6. Publiser branchen til GitHub

Trykk **Publish branch** øverst (eller **Push origin** hvis du har publisert før). Å **publisere/pushe** betyr å laste opp branchen og commitene dine fra din PC og opp til GitHub, slik at andre (og læreren) kan se dem. Nå ligger arbeidet ditt trygt på GitHub, i din egen branch.

### 7. Opprett en Pull Request (PR)

En **Pull Request** er en forespørsel om at endringene i branchen din skal bli gjennomgått og til slutt slått sammen (merget) inn i `main`.

1. Trykk **Create Pull Request** i GitHub Desktop — dette åpner GitHub i nettleseren.
2. Sjekk at **base: main** ← **compare: din branch**.
3. Beskrivelsesfeltet fylles automatisk ut med en mal — fyll inn:
   - Hvilken funksjon du har implementert
   - Hvordan du testet den
   - Eventuelle ting du er usikker på
   - `Closes #<issue-nummeret>` fra funksjonen du claimet i steg "Hva skal jeg gjøre?"
4. Trykk **Create Pull Request**.

### 8. Vent på review

Etter noen minutter kjører GitHub automatisk en test som sjekker at koden kompilerer — se etter en grønn hake ✅ (eller rød ❌) nederst på PR-siden. Deretter ser læreren gjennom koden din, kommenterer eller ber om endringer ved behov, og merger PR-en inn i `main` når den er godkjent. Får du kommentarer eller en rød ❌: gjør endringene i Arduino IDE, gå tilbake til GitHub Desktop og gjenta steg 5-6 (commit + push) på samme branch — de dukker automatisk opp i den samme PR-en.

### 9. Etterpå

Når PR-en er merget: bytt til `main` i GitHub Desktop og trykk **Fetch origin**/**Pull origin** for å få den ferdige koden din inn i `main` lokalt også. Skal du gjøre mer arbeid senere, start på nytt fra steg 2.

---

## Regler for pull requests

- ✅ Én PR per funksjon (eller en liten, avgrenset gruppe funksjoner du jobber med samtidig).
- ✅ Koden må kompilere før du åpner PR-en (den automatiske CI-sjekken må også bli grønn før læreren kan merge).
- ✅ Dokumentasjonskommentaren (se [ELEVOPPGAVER.md](ELEVOPPGAVER.md)) skal være med.
- ❌ Ikke rør andre sine funksjoner eller `loop()`/`setup()` med mindre dere har avtalt det.
- ❌ Ikke jobb direkte i `main`.
- ❌ Ikke merge din egen PR — det gjør læreren.

## Hvis GitHub Desktop varsler om konflikt

Det kan skje hvis flere har endret samme del av filen. Ikke få panikk — GitHub Desktop vil vise deg hvilke filer som har konflikt når du prøver å oppdatere branchen din fra `main` (f.eks. via "Branch → Update from main"). Åpne filen, se etter markeringer som `<<<<<<<`, `=======` og `>>>>>>>`, velg/rediger til riktig innhold, lagre, og commit på nytt i GitHub Desktop. Spør læreren om hjelp hvis du er usikker.
