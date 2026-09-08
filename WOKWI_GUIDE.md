# Guide: Simulere ESP32 i VS Code med Wokwi og Arduino CLI

Med Wokwi og Arduino CLI kan dere kompilere koden og kjøre en full interaktiv simulering av ESP32, NeoPixel LED-ringen og buzzeren direkte i Visual Studio Code — uten å koble til fysisk maskinvare!

---

## 1. Forberedelser og installasjon

### A. Installer Wokwi-utvidelsen i VS Code
1. Åpne **Extensions** i VS Code (`Ctrl+Shift+X` / `Cmd+Shift+X`).
2. Søk etter **Wokwi Simulator** (utgiver: *Wokwi*).
3. Trykk **Install**.

### B. Installer Arduino CLI

`arduino-cli.exe` kan lastes ned fra [Arduino CLI sine nettsider](https://arduino.cc/pro/cli) og legges i systemets PATH)

---

## 2. Engangskonfigurasjon av Arduino CLI

Åpne en terminal i VS Code (`Ctrl+Shift+\``) og kjør følgende tre kommandoer:
NB: Hvis den første kommandoen ikke virker så må du kanskje restarte VS Code
- Hvis det fortsatt ikke virker så har ikke arduino-cli havnet i PATH
- Da du installerte Arduino CLI havna det en exe-fil i C:\Program Files\Arduino CLI eller en lignende plassering
- Kopier plasseringen og legg den inn i PATH i Miljøvariabler. Det holder at den er enten i bruker eller system.
- Jeg tipper system er blokkert for redigering, for ellers ville den nok vært der allerede.
- Da må du igjen restarte VS-code for å oppdatere PATH som gjøres hver gang den starter opp.
      
```bash
# 1. Oppdater listen over kortpakker
arduino-cli core update-index

# 2. Installer ESP32-kortpakken
arduino-cli core install esp32:esp32

# 3. Installer Adafruit NeoPixel-biblioteket
arduino-cli lib install "Adafruit NeoPixel"
```

---

## 3. Aktiver Wokwi Lisens (Gratis)

1. Trykk `F1` (eller `Ctrl+Shift+P`) i VS Code.
2. Skriv og velg `Wokwi: Start Simulator`.
3. VS Code vil åpne en nettleser hvor du logger inn / godkjenner en gratis Wokwi-lisens for VS Code.
4. Når lisensen er aktivert, er du klar!

---

## 4. Slik bygger og simulerer du prosjektet

1. **Kompiler koden:**
   - Trykk `Ctrl+Shift+B` (eller `F1` → **Tasks: Run Build Task**).
   - Dette kjører `arduino-cli compile` og genererer filer i `build/`-mappen.

2. **Start simuleringen:**
   - Åpne filen [diagram.json](diagram.json) og trykk på **Play**-knappen i hjørnet av diagrammet, eller:
   - Trykk `F1` → **Wokwi: Start Simulator**.

3. **Interaksjon:**
   - Du vil se ESP32, NeoPixel-stripen (77 LED-er) og buzzeren i simuleringen.
   - Serial Monitor åpnes i VS Code slik at du kan sende kommandotekst (f.eks. "meny") eller lese utskrifter.

---

## Filstruktur for Wokwi i prosjektet

- `diagram.json`: Definerer de virtuelle komponentene (ESP32 DevKit V1, NeoPixel-stripe, Buzzer) og koblingene mellom dem.
- `wokwi.toml`: Forteller Wokwi hvor den kompilerte filen (`build/klokkeprosjekt_26.ino.elf` og `.bin`) ligger.
- `.vscode/tasks.json`: VS Code-byggeoppgave som bygger skissen med `arduino-cli`.
