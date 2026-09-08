# Lærerprosedyre – Gjennomgang og merging av elevarbeid

Denne filen beskriver arbeidsflyten *du* (læreren) følger for å ta imot, teste og slå sammen (merge) pull requests (PR-er) fra elevene inn i `main`. Se [README.md](README.md) for elevenes arbeidsflyt, [ELEVOPPGAVER.md](ELEVOPPGAVER.md) for hvilke funksjoner som skal implementeres og hvilken dokumentasjon som kreves, og **[GITHUB_OPPSETT.md](GITHUB_OPPSETT.md)** for engangs-oppsettet av organisasjonen, tilganger, branch protection og automatisk kompilerings-sjekk.

## Anbefalt oppsett før elevene starter

Se **[GITHUB_OPPSETT.md](GITHUB_OPPSETT.md)** for full sjekkliste (organisasjon, tilganger, branch protection, CI-sjekk og issue-basert funksjonsoversikt). Kort oppsummert:

1. **Beskytt `main`-branchen**: Require a pull request before merging + require den automatiske CI-sjekken (`compile`) skal være grønn før merge.
2. **Bruk GitHub Issues til å holde oversikt** over hvem som har tatt hvilken funksjon — elevene bruker den ferdige issue-malen "Velg en funksjon" (label `funksjon`) i stedet for et regneark eller en tavle i klasserommet.

---

## Gjennomgang av en Pull Request

Sjekkliste for hver PR før du merger:

1. **Omfang:** Sjekk fanen "Files changed" på GitHub — endrer PR-en kun den/de funksjonen(e) eleven skulle implementere? Rop varsku hvis `loop()`, `setup()` eller andres funksjoner er endret uten avtale.
2. **Signatur uendret:** Funksjonsnavn, parametere og returtype skal være identiske med det som lå i malen.
3. **`// Laget av:`** er fylt inn med elevens navn.
4. **Dokumentasjonskommentar** følger malen i [ELEVOPPGAVER.md](ELEVOPPGAVER.md) (Funksjon / Laget av / Hva gjør den / Parametere / Returverdi / Refleksjon), og refleksjonsdelen er reelt utfylt (ikke bare tomme fraser).
5. **Kompilerer koden?** PR-siden viser automatisk en ✅/❌ fra CI-sjekken `compile` (se [GITHUB_OPPSETT.md](GITHUB_OPPSETT.md)) — men test gjerne lokalt også (se under) før du godkjenner, siden CI kun sjekker at koden bygger, ikke at den faktisk fungerer på ringen.
6. Gi tilbakemelding som PR-kommentarer hvis noe må rettes, eller godkjenn/merge hvis alt er i orden.

---

## Slik henter og tester du en PR lokalt

1. Åpne GitHub Desktop, med dette repoet valgt.
2. Trykk på branch-menyen øverst → fanen **Pull Requests** → velg PR-en du vil teste. GitHub Desktop bytter automatisk til den branchen lokalt.
3. Åpne `klokkeprosjekt_26.ino` i Arduino IDE og kompiler (og gjerne last opp til en klokke) for å bekrefte at koden fungerer som forventet.
4. Finner du feil: skriv en kommentar i PR-en på GitHub (marker gjerne den aktuelle kodelinjen under "Files changed" → klikk `+`) og be eleven rette opp.

---

## Slik merger du en PR

**Anbefalt: via GitHub i nettleseren**

1. Åpne PR-en på github.com.
2. Se over "Files changed" én siste gang.
3. Trykk **Merge pull request** → **Confirm merge**.
4. Trykk **Delete branch** rett etterpå for å holde branch-listen ryddig.

**Alternativt: via GitHub Desktop**

1. Bytt til `main`-branchen.
2. Trykk **Branch → Merge into current branch...**, velg elevens branch, og bekreft.
3. Trykk **Push origin** for å laste den sammenslåtte `main` opp til GitHub.
4. Slett elevens branch (lokalt og på GitHub) når den er slått sammen.

---

## Håndtere konflikter mellom flere PR-er

Siden alle elever endrer den samme `.ino`-filen, kan en PR få "merge-konflikt" hvis `main` har endret seg (f.eks. fordi en annen PR ble merget først) siden eleven lagde branchen sin. GitHub varsler dette tydelig med "This branch has conflicts that must be resolved" på PR-siden.

- Konflikter oppstår oftest i **delte områder**: funksjonserklæringene øverst i filen, eller hvis to elever ved et uhell har begynt på samme funksjon.
- Konflikter *inni* en elevs egen funksjonskropp er sjeldne, siden ingen andre skal røre den.
- Løsning: be eleven oppdatere branchen sin fra `main` (i GitHub Desktop: **Branch → Update from main**), rydde opp i konfliktmarkeringene (`<<<<<<<` / `=======` / `>>>>>>>`) i Arduino IDE eller VS Code, teste at koden fortsatt kompilerer, og pushe på nytt.

---

## Etter merge

- Lukk issuet eleven opprettet for funksjonen (skjer automatisk hvis PR-beskrivelsen inneholder `Closes #<issue-nummer>`, se PR-malen).
- Vurder å gjøre en helhetlig kompilering av `main` med jevne mellomrom etter hvert som flere funksjoner fylles inn — når flere reelle implementasjoner møtes (f.eks. `fyllPlan()` + `planIndex()` + `hentGjeldendeFag()`), kan det dukke opp integrasjonsfeil som ikke var synlige da hver funksjon ble testet alene.
- Last opp `main` til en fysisk klokke innimellom, så elevene får se resultatet av arbeidet sitt i praksis.

## Tips for effektiv saksbehandling

- Sett av faste tidspunkt (f.eks. daglig) til å gå gjennom åpne PR-er, så elevene ikke venter unødvendig lenge.
- Bruk GitHub sin "Suggested changes"-funksjon i code review for raskt å foreslå små rettelser direkte i koden.
- Lukk (ikke slett) PR-er som blir forkastet, med en kommentar om hvorfor — det er nyttig historikk.
